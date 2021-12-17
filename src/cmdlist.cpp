#include "cmdlist.h"
#include "commandline.h"

CmdList::CmdList(CmdWidget *parent) : QListWidget(parent)
{
    root = parent->root;
    rootParent = parent;

    connect(this, &CmdList::doubleClicked, this, [this]{ rootParent->cmd->completeArg(); });

    updateWidgetStyle();
}

void CmdList::loadCommands() {
    rootParent->cmd->clear();
    clear();

    for (const auto &name : rootParent->COMMAND_LIST) {
        addItem(name.first.c_str());
    }

    setCurrentItem(item(0));
    rootParent->show();
}

void CmdList::redrawCommands() {
    clear();

    rootParent->cmd->cursorWordBackward(true);
    string searchQuery = rootParent->cmd->selectedText().toLower().toStdString();
    rootParent->cmd->cursorWordForward(true);

    /*string curComArg = "";
    string commandName = "";
    vector<string> out = rootParent->cmd->splitCommand(rootParent->cmd->text().toLower().toStdString());
    if (out.size() > 0 && rootParent->COMMAND_LIST.find(out[0]) != rootParent->COMMAND_LIST.end()) {
        string index = out[0];
        commandName = out[0];
        curComArg = rootParent->COMMAND_LIST[index];
    }*/

    //if (curComArg == "") {
    vector<string> out = rootParent->cmd->splitCommand(rootParent->cmd->text().toLower().toStdString());
    if (out.size() < 2) {
        for (const auto & command : rootParent->COMMAND_LIST) {
            string cname = QString(command.second.c_str()).toLower().toStdString();

            if ( cname.find(searchQuery) != string::npos ) {
                QString name = command.first.c_str();
                addItem(name);
            }
        }
    }
    /*}
    else {
        if (curComArg.find("int=") != string::npos) {
            replaceStr(curComArg, "int=", "");
            vector<int> args = {};
            try {
                args = betwStrToVectorInt(curComArg);
            }
            catch (const std::exception& e) {
                log("Int arg error!");
            }
            for (const auto &arg : args) {
                string cname = to_string(arg);
                if (searchQuery == " " || searchQuery == commandName
                    || searchQuery == commandName + " ") {
                    searchQuery = "";
                }

                if ( cname.find(searchQuery) != string::npos ) {
                    QString name = cname.c_str();
                    addItem(name);
                }
            }
        }
    }*/

    //sortItems();
    rootParent->resizeWidgetByListItems();
    setCurrentItem(item(0));
}

void CmdList::updateWidgetStyle() {
    QString style = QString("QListWidget { color: %1; border: none; background: %2; }")
                    .arg(root->theme["clipboardFontColor"].c_str(),
                         root->theme["clipboardBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->clipboardFontFamily.c_str());
    fnt.setPixelSize(root->cfg->clipboardFontSize);
    setFont(fnt);

    setGeometry(0, root->cfg->cmdListTopPadding, root->cfg->cmdListWidth, root->cfg->cmdListHeight);
}
