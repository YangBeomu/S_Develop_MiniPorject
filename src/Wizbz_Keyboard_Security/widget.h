#ifndef WIDGET_H
#define WIDGET_H

#include "pch.h"
#include "util.hpp"
#include "keyboardSecurity.h"

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    static constexpr uint32_t THREAD_TICK = 100;

    //other class
    unique_ptr<KeyboardSecurity> ksPtr_{};

    //process var
    std::vector<std::string> processNames_{};
    std::string processName_{};

    //control var
    std::thread syncThreadHandle_{};
    std::mutex controlMtx_{};

    bool isRunning_{};


    //control
    bool init();

    void RefreshProcesses();

    //state
    void SwitchState(QWidget* widget);

    //log
    void WarningMsg(const std::string& msg);
    void ErrorMsg(const std::string& msg);

    //thread
    void SyncThreadFunc();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    std::string GetProcessName();

private slots:
    void on_pbRefrsh_pressed();
    void on_pbAttach_pressed();
    void on_pbDetach_pressed();

    void on_leProcessSearch_textChanged(const QString &arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
