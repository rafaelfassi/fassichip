#ifndef NORWEGIANWOODPLUGIN_H
#define NORWEGIANWOODPLUGIN_H

#include <QStylePlugin>


class NorwegianWoodPlugin : public QStylePlugin
{
public:
    QStringList keys() const;
    QStyle *create(const QString &key);
};

#endif
