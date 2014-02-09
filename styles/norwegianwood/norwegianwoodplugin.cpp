#include <QtGui>

#include "norwegianwoodplugin.h"
#include "norwegianwoodstyle.h"

QStringList NorwegianWoodPlugin::keys() const
{
    return QStringList() << "NorwegianWood";
}

QStyle *NorwegianWoodPlugin::create(const QString &key)
{
    if (key.toLower() == "norwegianwood")
        return new NorwegianWoodStyle;
    return 0;
}

Q_EXPORT_PLUGIN2(NorwegianWoodPlugin, NorwegianWoodPlugin)
