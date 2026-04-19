#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "device.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAnalyzeClicked();
    void onClearClicked();

private:
    QByteArray parseHexInput(const QString& input);
    Device* identifyDevice(const QByteArray& data);
    void showError(const QString& message);

    QLineEdit* m_inputEdit;
    QTextEdit* m_outputEdit;
    QPushButton* m_analyzeButton;
    QPushButton* m_clearButton;
    QLabel* m_statusLabel;

    QList<Device*> m_devices;
};

#endif // MAINWINDOW_H
