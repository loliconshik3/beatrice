#include "mainwindow.h"
#include "filelistinsidelist.h"
#include "filelistwidget.h"
#include "filelistsearch.h"
#include "filelisttext.h"
#include "filelistinfo.h"
#include "replacetext.h"
#include "commandline.h"
#include "findwidget.h"
#include "clipboard.h"
#include "infopanel.h"
#include "filelist.h"
#include "findtext.h"
#include "textbox.h"
#include "utils.h"

#include <QApplication>

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
  Идеи для редактора.

  +• Dobavit pri napisanii " v "" - peremeshenie cursora vleva
  +• Dobavit na ctrl+p otkritie spiska filov, kotorie otkriti shyas

  • Сделать чтобы ктрл + низ\верх двигало скролл бар
  +• Можно добавить в файл листе доп окно, которое будет отображать текст из файла
    Иего, если что, можно будет выделить и скопировать (мб)
  +• При нажатии Escape в текстбоксе - должно сбрасываться выделение текста
  +• При копировании текста записывать скопированный текст в список
    А при вставке через Ctrl+Shift+v - Показывать список всех скопированных строк
    и давать выбор какую вставить
  +• Исправить движение курсора в скобках при нажатии левой скобки
    Добавить при нажатии скобки с выделенным текстом - выделять этот текст в скобки
    также и с кавычками. Будет удобненько
  • При вводе команды, нажимая ктрл+таб - пусть появляется на месте инфопанели
    список (горизонтальный) комманд, из которых можно выбрать. А при изменении текста
    в коммандлайне, этот список будет фильтроваться
  • Можно добавить при выделении текста, если это цветовой код, то менять цвет квадратика в инфопанели
  +• Вместо сразу открытия последнего файла, можно просто подгружать отдельный лист на ctrl+???
    Который будет последние 5 файлов отображать

  • В конфиг добавить параметр: открывать ли сразу все файлы предыдущей сессии
    ну и, соответственно, хранить список в отдельном файле. в идеале - записывать,
    когда беатрис закрывается

  +• Можно сделать вкладки. Сделать класс для файла, в котором вся инфа о нем будет
    И сделать список этих файлов. жмешь ктрл+н - создаёшь новый класс
    открываешь новый файл - создаёшь новый класс
    жмёшь ктрл+ц - файл закрывается, класс удаляется.

  +• Дополнение кавычек и скобок
  • Своё контекстное меню для текстбокса и коммандлайна
  +• Конфиг (Ваще, в жсон файлы можно будет раздел comment добавить, чтобы пояснять чо по чом)
  +• Ну, как обычно, управление через горячие клавиши.
  +• Окно со списком файлов в текущей директории
  +• Окно со списком последних 5 файлов (можно будет перемещаться как стрелками, так и юзать мышь)
  • Окно со списком последних 5 папок
  +• Восстановление открытых файлов при перезаходе
  • Добавление ; при переносе строки, если в файле синтаксиса так указано
  +• Подсветка синтаксиса
  +• Окно с информацией
  • Окно с помощью
  +• Консольные команды
  +• Пусть последняя команда в плейсхолдер ставиться и на таб её можно будет быстро вызвать
  • Возможность для отдельных типов файлов настроить свой компилятор. (а в идеале для отдельных проектов, как в висуалкоде)
  • Понятные файлы подсветки синтаксиса
  • Макросы(!!!)
  +• Цветовые темы
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

void helpCommand() {
    cout << "beatrice | by loliconsik3" << endl;
    cout << "<file>" << endl;
    cout << "    open file in editor" << endl;
    cout << "-h" << endl;
    cout << "    show this text" << endl;
}

int main(int argc, char *argv[])
{
    if (argc > 1 and argv[1] == QString("-h")) {
        helpCommand();
        return 0;
    }

    system("mkdir ~/.config/beatrice");
    system("mkdir ~/.config/beatrice/cache");
    system("mkdir ~/.config/beatrice/macros");

    QApplication app(argc, argv);
    MainWindow window;

    FileListWidget flwidget(&window);
    FileList filelist(&flwidget);
    FileListSearch flsearch(&flwidget);
    FileListInfo flinfo(&flwidget);
    FileListText fltext(&flwidget);
    FileListInsideList flinsidelist(&flwidget);

    Textbox textbox(&window);
    CommandLine commandline(&window);
    InfoPanel infopanel(&window);
    Clipboard clipboard(&window);

    FindWidget findwidget(&window);
    FindText findtext(&findwidget);
    ReplaceText replacetext(&findwidget);

    QGridLayout *layout = new QGridLayout;

    flwidget.flinsideList = &flinsidelist;
    window.flwidget     = &flwidget;
    flwidget.filelist   = &filelist;
    flwidget.flsearch   = &flsearch;
    flwidget.flinfo     = &flinfo;
    flwidget.fltext     = &fltext;
    window.commandline  = &commandline;
    window.findwidget   = &findwidget;
    window.infopanel    = &infopanel;
    window.textbox      = &textbox;
    window.clip         = &clipboard;
    findwidget.findtext = &findtext;
    findwidget.replacetext = &replacetext;

    layout->addWidget(window.textbox, 0, 0);
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
    findwidget.hide();
    window.infopanel->updateText();

    if (argc > 1) {
        //std::string test = argv[1];
        //if (find)
        //Сделать чтобы при вводе аргумента, если отсутствует знак /, то не открывать файл, а менять тайтл
        // на текущую дерикторию + файлнейм
        window.openFile(argv[1]);
    }
    else {
        window.newFile();
    }
    system("clear");

    return app.exec();
}
