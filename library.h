#pragma once
#include <memory>
#include <QDir>
#include <vector>
#include <QImage>

class Library {
public:
    virtual int count() const = 0;
    virtual QImage operator[](int n) const = 0;
    virtual void refresh() = 0;
};

class LibraryList: public Library {
public:
    LibraryList();
    LibraryList(std::vector<std::string> paths);
    LibraryList(QStringList paths);
    int count() const override;
    QImage operator[](int n) const override;
    void add(std::unique_ptr<Library> lib);
    void refresh() override;

private:
    std::vector<std::unique_ptr<Library>> lst;
};

class Dir: public Library {
public:
    Dir(QString path, QStringList filters);
    int count() const override;
    QImage operator[](int n) const override;
    void refresh() override;
private:
    QDir qdir;
    int cnt;
};
