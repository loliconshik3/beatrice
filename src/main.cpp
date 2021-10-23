#include "mainwindow.h"
#include "filelistwidget.h"
#include "filelistsearch.h"
#include "commandline.h"
#include "infopanel.h"
#include "fileslist.h"
#include "textbox.h"
#include "utils.h"

#include <QApplication>
#include <QMessageBox>

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

/*
 Готовые горячие клавиши

 * Alt+r - удаляет текущую строку
 * Ctrl+q - Закрывает редактор
 * Alt+e - Меняет фокус с текстбокса на командную строку и обратно
 * Ctrl+o - Открывает файл
 * Ctrl+n - Создаёт новый файл
 * Ctrl+s - Сохраняет файл
 * Ctrl+i - Открывает окно about
 * Alt+d - Дублирует текущую строку
 * Ctrl+Tab - Добавляет таб в начало строки
 * Ctrl+Delete (в файллисте) - Удаляет выделенный файл
 * Ctrl+k - Открывает директорию
 * Ctrl+Shift+s - Сохранить файл как
 * Ctrl+p - Open tab files
 * Alt+s - Open last files
 * Alt+Up - Открывает верхний открытый файл
 * Alt+Down - Открывает нижний открытый файл
 *
 *
*/

/*
 Функции
 ! - не устраивает
 + - устраивает
 ? - не уверен

 +* Сохранение файла
 +* Открытие файла
 +* Перемещение между папками
 !* Открытие папки
 *
 *
 *
*/

/*
  Идеи для редактора.

  +• Dobavit pri napisanii " v "" - peremeshenie cursora vleva
  +• Dobavit na ctrl+p otkritie spiska filov, kotorie otkriti shyas

  • Можно добавить при выделении текста, если это цветовой код, то менять цвет квадратика в инфопанели
  +• Вместо сразу открытия последнего файла, можно просто подгружать отдельный лист на ctrl+???
    Который будет последние 5 файлов отображать

  • Можно сделать вкладки. Сделать класс для файла, в котором вся инфа о нем будет
    И сделать список этих файлов. жмешь ктрл+н - создаёшь новый класс
    открываешь новый файл - создаёшь новый класс
    жмёшь ктрл+ц - файл закрывается, класс удаляется.

  +• Дополнение кавычек и скобок
  • Своё контекстное меню для текстбокса и коммандлайна
  • Конфиг (Ваще, в жсон файлы можно будет раздел comment добавить, чтобы пояснять чо по чом)
  • Ну, как обычно, управление через горячие клавиши.
  +• Окно со списком файлов в текущей директории
  • Окно со списком последних 5 файлов (можно будет перемещаться как стрелками, так и юзать мышь)
  • Окно со списком последних 5 папок
  +• Восстановление открытых файлов при перезаходе
  • Добавление ; при переносе строки, если в файле синтаксиса так указано
  • Подсветка синтаксиса
  +• Окно с информацией
  • Окно с помощью
  +• Консольные команды
  +• Пусть последняя команда в плейсхолдер ставиться и на таб её можно будет быстро вызвать
  • Возможность для отдельных типов файлов настроить свой компилятор. (а в идеале для отдельных проектов, как в висуалкоде)
  • Понятные файлы подсветки синтаксиса
  • Макросы(!!!)
  • Цветовые темы
  • Поиск и реплейс
  +• Добавить возможность перемещаться между директориями в файллисте (вариант back в самом низу, который в предыдущую перемещает)
    И чтобы директории отображались, но с каким-то отличительным знаком
    И когда жмешь на директорию, то окно не закрывается, текущая директория заменяется, перерисовывается и тадам)
  +• Номер строки
  +• Приличный дизайн
  +• Чтобы в консоли можно было ввести путь до файла при запуске и файл сразу откроется
  +• Скейл текста на ктрл++ и ктрл+-
  +• Звёздочка, если файл не сохранён
  +• Скейл интерфейса
*/

void readJson()
{
    QString val;
    QFile file((getHomeDir()+"/.beatrice/config.json").c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    qWarning() << val;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();
    QJsonValue value = sett2.value(QString("window-minimum-width"));
    qWarning() << value;
    QJsonObject item = value.toObject();
    qWarning() << QObject::tr("QJsonObject of description: ") << item;

    /* in case of string value get value and convert into string*/
    qWarning() << QObject::tr("QJsonObject[appName] of description: ") << item["description"];
    QJsonValue subobj = item["description"];
    qWarning() << subobj.toString();

    /* in case of array get array and convert into string*/
    qWarning() << QObject::tr("QJsonObject[appName] of value: ") << item["imp"];
    QJsonArray test = item["imp"].toArray();
    qWarning() << test[1].toString();
}

void helpCommand() {
    cout << "beatrice | by loliconsik3" << endl;
    cout << "<file>" << endl;
    cout << "    open file in editor" << endl;
    cout << "-h" << endl;
    cout << "    show this text" << endl;
}

int main(int argc, char *argv[])
{
    //std::cout << argv[1] << std::endl;

    if (argc > 1 and argv[1] == QString("-h")) {
        helpCommand();
        return 0;
    }

    system("mkdir ~/.beatrice");
    system("mkdir ~/.beatrice/cache");
    //system("cd ~/.beatrice && git clone https://github.com/loliconshik3/ledit");
    writeStandartConfig();

    QApplication app(argc, argv);
    MainWindow window;

    MainWindow::Textbox textbox(&window);
    MainWindow::CommandLine commandline(&window);
    MainWindow::InfoPanel infopanel(&window);

    MainWindow::FileListWidget flwidget(&window);
    MainWindow::FileListWidget::FilesList fileslist(&flwidget);
    MainWindow::FileListWidget::FileListSearch flsearch(&flwidget);

    QGridLayout *layout = new QGridLayout;

    window.commandline = &commandline;
    window.infopanel = &infopanel;
    window.textbox = &textbox;
    window.flwidget = &flwidget;
    flwidget.fileslist = &fileslist;
    flwidget.flsearch = &flsearch;

    layout->addWidget(window.textbox, 0, 0);
    //layout->addWidget(&testbox);
    layout->addWidget(window.commandline, 2, 0);
    layout->addWidget(window.infopanel, 1, 0);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);

    window.setLayout(layout);

    window.show();
    window.textbox->show();
    window.infopanel->show();
    window.commandline->show();
    flwidget.hide();
    window.infopanel->updateText();
    window.UpdateTitle();

    //QMessageBox::StandardButton reply;
    //reply = QMessageBox::question(&window, "Loader", "Load last file?",
    //                              QMessageBox::No|QMessageBox::Yes);
    //if (reply == QMessageBox::Yes) {
    //}

    if (argc > 1) {
        //std::string test = argv[1];
        //if (find)
        //Сделать чтобы при вводе аргумента, если отсутствует знак /, то не открывать файл, а менять тайтл
        // на текущую дерикторию + файлнейм
        window.OpenFile(argv[1]);
    }
    else {
        //window.loadLastFile();
    }

    //readJson();

    return app.exec();
}
