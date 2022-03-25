#include "effectsilencefactory.h"
#include "silenceplugin.h"
#include "settingsdialog.h"

const EffectProperties EffectSilenceFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Silence Removal Plugin");
    properties.shortName = "silence";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectSilenceFactory::create()
{
    return new SilencePlugin();
}

void EffectSilenceFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(silence, EffectSilenceFactory)
#endif
