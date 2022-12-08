#include <algorithm>
#include <ranges>

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QSet>

// Globals
// That's bad, boouh
class Directory;
QSet<Directory *> matchingDirs;
QList<Directory *> allDirs;

struct File
{
    QString name;
    qint64 size;
};

struct Directory {
    QString name;
    QMap<QString, Directory *> dirs;
    QMap<QString, File> files;
    qint64 m_size = 0;
    Directory *m_parent = nullptr;



public:
    Directory() :
        name("/")
    {
        allDirs.append(this);
    };
    explicit Directory(const QString &name, Directory *parent);

    qint64 size() const;
    void setSize(qint64 newSize);

    void touch(const QString &name, qint64 size) {
        if (files.contains(name)) {
            files[name].size = size;
        } else {
            files.insert(name, {name, size});
        }

        updateSize();
    }

    void mkdir(const QString &name) {
        if(dirs.contains(name))
            return;

        dirs.insert(name, new Directory(name, this));
    }
    Directory *parent() const;

    void updateSize()
    {
        m_size = 0;
        for (const auto &dir : dirs.values()) {
            m_size += dir->m_size;
        }
        for (const auto &file : files.values()) {
            m_size += file.size;
        }

        if (m_parent) {
            m_parent->updateSize();
        }

        if (m_size <= 100'000) {
            matchingDirs.insert(this);
        } else {
            matchingDirs.remove(this);
        }
    }
};

class FileSystem {
    public:
    Directory root;
    Directory *pwd = &root;


    void cd(const QString &path) {
        if (path.startsWith('/'))
            pwd = &root;

        QStringList dirs = path.split('/', Qt::SkipEmptyParts);

        for (const auto &dir : dirs){
            if (dir == ".")
                continue;

            if (dir == "..") {
                pwd = pwd->parent();
                Q_ASSERT_X(pwd != nullptr, "cd", "cd .. into emptiness");
                continue;
            }

            pwd = pwd->dirs.value(dir);
            Q_ASSERT_X(pwd != nullptr, "cd", "cd into a non existent directory");
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");

    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    FileSystem fs;

    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();
        if(line.startsWith('$')) { //command
            if (line.mid(2, 2) == "cd") {
                fs.cd(line.mid(5));
            } else if (line.mid(2, 2) == "ls") {

            } else {
                qFatal("Unknown command %s", line.constData());
            }
        } else {
            const auto split = line.split(' ');
            Q_ASSERT(split.size() == 2);

            if (split[0] == "dir") {
                fs.pwd->mkdir(split[1]);
            } else {
                bool ok = false;
                qint64 size = split[0].toInt(&ok);
                Q_ASSERT(ok);
                fs.pwd->touch(split[1], size);
            }
        }


    }

    // PART 1
    qint64 sum = 0;
    for (const auto &dir: matchingDirs) {
        sum += dir->size();
    }
    qInfo() << "PART 1:" << sum;

    // PART 2
    QList<qint64> sizes;
    sizes.resize(allDirs.size());
    std::ranges::transform(allDirs, sizes.begin(), [](Directory *dir){return dir->size();});
    std::ranges::sort(sizes);
    const qint64 toFree = fs.root.size() - 40'000'000;
    auto it = std::ranges::lower_bound(sizes, toFree);
    qInfo() << "Part 2:" << (*it);
    return 0;
}

Directory *Directory::parent() const
{
    return m_parent;
}

Directory::Directory(const QString &name, Directory *parent):
    name(name),
    m_parent(parent)
{
    allDirs.append(this);
}

qint64 Directory::size() const
{
    return m_size;
}

void Directory::setSize(qint64 newSize)
{
    m_size = newSize;
}
