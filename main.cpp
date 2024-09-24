#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <fstream>
#include <QTextStream>
#include <QFile>

#include "variantmaptablemodel.h"

static std::string work_file_name = "work-file.txt";

void showMessageBox(QQmlEngine& engine, const char* messageText)
{
    engine.rootContext()->setContextProperty("reasonText", messageText);
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/MessageBox.qml")));
    component.create();
}

typedef std::pair<QFile*, bool> OpenedQFile_t;

OpenedQFile_t tryOpenFile(QQmlEngine& engine, const std::string &fileName)
{
    if (fileName.empty())
        return {nullptr, false};

    if (!QFile::exists(QString::fromStdString(fileName))) {
        showMessageBox(engine, "Ошибка: файл не существует");
        return {nullptr, false};
    }

    QFile *file = new QFile(QString::fromStdString(fileName));

    try {
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
            throw std::system_error(errno, std::system_category(),
                                    "Failed to open file " + fileName);
    } catch (const std::system_error &e) {
        std::string error_str = "Ошибка: ";
        error_str.append(strerror(errno));
        error_str.append(" (").append(e.code().category().name());
        error_str.append(":");
        error_str.append(std::to_string(e.code().value())).append(")");
        error_str.append("\nНе удалось загрузить файл");
        showMessageBox(engine, error_str.c_str());
        return {nullptr, false};
    }

    if (file->atEnd()) {
        showMessageBox(engine, "Ошибка: файл пустой");
        return {nullptr, false};
    }

    return {file, true};
}

void fillTableModelFromFile(OpenedQFile_t &&qFile, VariantMapTableModel &tableModel)
{
    if (!qFile.second)
        return;

    QTextStream in(qFile.first->readAll().replace(';', ' ').trimmed());

    QString userId, userName, phoneNumber;

    QVariantMap user;

    while (!in.atEnd()) {
        in >> userId >> userName >> phoneNumber;

        user.insert("User ID", userId);
        user.insert("User name", userName);
        user.insert("Phone number", phoneNumber);

        tableModel.addRow(user);

        user.clear();
    }

    qFile.first->close();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<VariantMapTableModel>("UserTableModel", 0, 1, "UserTableModel");

    QQmlApplicationEngine mainEngine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&mainEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    VariantMapTableModel userTableModel;

    userTableModel.registerColumn(new SimpleColumn("User ID"));
    userTableModel.registerColumn(new SimpleColumn("User name"));
    userTableModel.registerColumn(new SimpleColumn("Phone number"));

    fillTableModelFromFile(tryOpenFile(mainEngine, work_file_name), userTableModel);

    mainEngine.rootContext()->setContextProperty("usertablemodel", &userTableModel);

    mainEngine.load(url);

    if (mainEngine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
