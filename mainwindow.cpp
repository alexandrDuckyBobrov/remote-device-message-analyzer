#include "mainwindow.h"
#include "korshun.h"
#include "drofa.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    m_devices.append(new Korshun());
    m_devices.append(new Drofa());

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QGroupBox* inputGroup = new QGroupBox("Ввод сообщения");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);

    QLabel* inputLabel = new QLabel("Введите шестнадцатерчиные байты через пробел:");
    inputLayout->addWidget(inputLabel);

    m_inputEdit = new QLineEdit();
    m_inputEdit->setPlaceholderText("3A 46 30 31 31 30 53 32 43 30 38 21");
    inputLayout->addWidget(m_inputEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_analyzeButton = new QPushButton("Анализировать");
    m_clearButton = new QPushButton("Очистить");
    buttonLayout->addWidget(m_analyzeButton);
    buttonLayout->addWidget(m_clearButton);
    inputLayout->addLayout(buttonLayout);

    mainLayout->addWidget(inputGroup);

    QGroupBox* outputGroup = new QGroupBox("Результаты анализа");
    QVBoxLayout* outputLayout = new QVBoxLayout(outputGroup);

    m_outputEdit = new QTextEdit();
    m_outputEdit->setReadOnly(true);
    m_outputEdit->setFont(QFont("Courier New", 10));
    outputLayout->addWidget(m_outputEdit);

    mainLayout->addWidget(outputGroup);

    m_statusLabel = new QLabel("Готов к приему сообщений");
    statusBar()->addWidget(m_statusLabel);

    connect(m_analyzeButton, &QPushButton::clicked, this, &MainWindow::onAnalyzeClicked);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    setWindowTitle("Анализатор сообщений устройств Коршун и Дрофа");
    setMinimumSize(600, 400);


}

MainWindow::~MainWindow()
{
    qDeleteAll(m_devices);
}

void MainWindow::onAnalyzeClicked()
{
    QString inputText = m_inputEdit->text().trimmed();

    if (inputText.isEmpty())
    {
        showError("Введите данные для анализа");
        return;
    }

    QByteArray data = parseHexInput(inputText);

    if (data.isEmpty())
    {
        showError("Неверный формат ввода. ИСпользуйте шестнадцатиричные байты через пробел.");
        return;
    }

    Device* device = identifyDevice(data);

    if (!device)
    {
        showError("Не удалось определить устройство. Сообщение не соответстует ни одному протоколу");
        return;
    }

    if (device->parse(data))
    {
        m_outputEdit->append("====================");
        m_outputEdit->append("Получено сообщение:");
        m_outputEdit->append("HEX: " + inputText.toUpper());
        m_outputEdit->append("");
        m_outputEdit->append(device->getStateString());
        m_outputEdit->append("");

        m_statusLabel->setText("Сообщение успешно обработано: " + device->getName());

    }
    else
    {
        showError("Ошибка анализа сообщения для устройствва " + device->getName() + "\nСообщение не соответствует протоколу или содержит неверные значения.");
        m_statusLabel->setText("Ошибка обработки сообщения");
    }
}

void MainWindow::onClearClicked()
{
    m_inputEdit->clear();
    m_outputEdit->clear();
    m_statusLabel->setText("Готов к приему сообщений");
}

QByteArray MainWindow::parseHexInput(const QString& input)
{
    QByteArray result;

    QStringList hexBytes = input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    for (const QString& hexByte : hexBytes)
    {
        bool ok;
        int byte = hexByte.toInt(&ok, 16);

        if (!ok || byte < 0 || byte > 0xFF)
        {
            return QByteArray();
        }

        result.append(static_cast<char>(byte));
    }

    return result;
}

Device* MainWindow::identifyDevice(const QByteArray& data)
{
    if(data.isEmpty())
        return nullptr;

    if (data.size() >= 2 &&
        static_cast<unsigned char>(data[0]) == 0x3A &&
        static_cast<unsigned char>(data[data.size() - 1]) == 0x21)
    {
        return m_devices[0]; // Коршун
    }

    if (data.size() >= 4 &&
        static_cast<unsigned char>(data[0]) == 0xFF &&
        static_cast<unsigned char>(data[1]) == 0XFF)
    {
        return m_devices[1]; // Дрофа
    }

    return nullptr;
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::warning(this, "Ошибка", message);
    m_outputEdit->append("ОШИБКА: " + message);
}