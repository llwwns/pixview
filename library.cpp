#include "library.h"
#include <QImage>


QStringList defaultFilters() {
	QStringList filters;
	filters<<"*.bmp"<<"*.gif"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.tiff";
    return filters;
}

const QStringList filters = defaultFilters();

LibraryList::LibraryList() {
}

LibraryList::LibraryList(std::vector<std::string> paths):Library() {
    for (auto s: paths) {
        add(std::unique_ptr<Library>(new Dir(QString::fromStdString(std::move(s)), filters)));
    }
}

LibraryList::LibraryList(QStringList paths):Library() {
    for (auto s: paths) {
        add(std::unique_ptr<Library>(new Dir(std::move(s), filters)));
    }
}


void LibraryList::add(std::unique_ptr<Library> lib) {
    lst.push_back(std::move(lib));
}

int LibraryList::count() const {
    int c = 0;
    for (auto &l: lst) {
        c += l->count();
    }
    return c;
}

QImage LibraryList::operator[](int n) const {
    int c = 0;
    for (auto &l: lst) {
        int i = l->count();
        if (n - c < i) {
            return (*l)[n - c];
        }
        c += i;
    }
    return QImage();
}

void LibraryList::refresh() {
    for (auto &l: lst) {
        l->refresh();
    }
}

Dir::Dir(QString path, QStringList filters): qdir(path) {
    qdir.setNameFilters(std::move(filters));
    cnt = qdir.count();
}


int Dir::count() const {
    return cnt;
}

QImage Dir::operator[](int n) const {
    if (n >= cnt) {
        return QImage();
    }
    try {
        auto file_name = qdir[n];
        auto full_path = qdir.absoluteFilePath(file_name);
        auto image = QImage(full_path);
        return image;
    } catch(...) {
        return QImage();
    }
}

void Dir::refresh() {
    qdir.refresh();
    cnt = qdir.count();
}
