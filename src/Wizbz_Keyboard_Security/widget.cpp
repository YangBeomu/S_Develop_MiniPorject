#include "pch.h"

#include "widget.h"
#include "ui_widget.h"

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    if(!init()) throw runtime_error("Failed to init");
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::init() {
    try {
        RefreshProcesses();
        SwitchState(ui->pbDetach);

    }catch(const exception& e) {
        WarningMsg(string("<Init> ").append(e.what()));
        return false;
    }

    return true;
}

void Widget::RefreshProcesses() {
    try {
        vector<string> processNames = utils::GetProcessNames();
        if(processNames.empty()) throw runtime_error("Failed to call get process names");

        if(!processNames_.empty()) {
            processNames_.clear();
            //item deleted?
            ui->lwProcess->clear();
        }

        processNames_ = processNames;

        for(const auto& pn : processNames_) {
            QListWidgetItem* items = new QListWidgetItem(QString(pn.c_str()));
            ui->lwProcess->addItem(items);
        }

    }catch(const exception& e) {
        WarningMsg(string("<RefreshProcesses> ").append(e.what()));
    }
}

void Widget::SwitchState(QWidget* widget) {
    widget->setEnabled(!widget->isEnabled());
}

void Widget::WarningMsg(const string& msg) {
    cerr<<"<Widget> "<<msg<<endl;
}

void Widget::ErrorMsg(const string& msg) {
    cerr<<"<Widget> "<<msg<<endl;

    exit(1);
}

void Widget::on_pbRefrsh_pressed() {
    RefreshProcesses();
}


void Widget::on_pbAttach_pressed() {
    try {
        if(ui->lwProcess->currentItem() == nullptr) throw runtime_error("Failed to get current item.");
        processName_ = ui->lwProcess->currentItem()->text().toStdString();
        if(processName_.empty()) throw runtime_error("Failed to get process name.");

        if(ksPtr_ != nullptr) throw runtime_error("Failed to get keyboard security pointer.");

        //한세트
        ksPtr_ = make_unique<KeyboardSecurity>(processName_);
        //isRunning_ = true;
        //syncThreadHandle_ = std::thread(&Widget::SyncThreadFunc, this);

        SwitchState(ui->pbAttach);
        SwitchState(ui->pbDetach);

        MessageBoxA(NULL, processName_.c_str(), "Attached", NULL);

    }catch(const exception& e) {
        WarningMsg(string("<on_pbAttach_pressed> ").append(e.what()));
    }
}


void Widget::on_pbDetach_pressed()
{
    try{
        processName_.clear();

        if(ksPtr_ == nullptr) throw runtime_error("Failed to get keyboard security pointer.");

        ksPtr_.reset();

        unique_lock<mutex> lck(controlMtx_);
        isRunning_ = false;

        if(syncThreadHandle_.joinable()) syncThreadHandle_.join();

        SwitchState(ui->pbAttach);
        SwitchState(ui->pbDetach);

    }catch(const exception& e) {
        WarningMsg(string("<on_pbDetach_pressed> ").append(e.what()));
    }

    MessageBoxA(NULL, "Detached", "Wizbz", NULL);
}


void Widget::on_leProcessSearch_textChanged(const QString &arg1)
{
    auto items = ui->lwProcess->findItems(arg1, Qt::MatchFlag::MatchContains);
    bool hidden{};

    if(!items.empty()) hidden = true;

    if(hidden) {
        for(int idx=0; idx<ui->lwProcess->count(); idx++)
            ui->lwProcess->item(idx)->setHidden(hidden);

        for (auto item : items)
            item->setHidden(false);
    }
}

//thread
void Widget::SyncThreadFunc() {
    try {
        if(this->ksPtr_ == nullptr) throw runtime_error("keyboard security nullptr.");

        while(isRunning_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TICK));

            auto encryptedData = this->ksPtr_.get()->ExportRegisteredVkCodes(KeyboardSecurity::ENCRYPTED);
            if(encryptedData.empty()) continue;

            QString encryptedTxt{};

            for(const auto& ed : encryptedData)
                encryptedTxt.append(to_string(ed));


            if(encryptedTxt.isEmpty()) continue;
            ui->ptEncryptedInput->setPlainText(encryptedTxt);
        }
    }catch(const exception& e) {
        WarningMsg(string("<SyncThreadFunc> ").append(e.what()));
    }

}

