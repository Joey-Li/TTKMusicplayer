include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decodercdaudiofactory.h \
           decoder_cdaudio.h \
           settingsdialog.h

SOURCES += decodercdaudiofactory.cpp \
           decoder_cdaudio.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libcddb/include \
               $$EXTRA_PREFIX/libcdio/include \
               $$EXTRA_PREFIX/libcdio_paranoia/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libcdio/lib -lcdio$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libcdio/lib -lcdio_paranoia$$STATIC_LIBRARY_SUFFIX -lcdio_cdda$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libcdio/lib -lcdio -lcdio_paranoia -lcdio_cdda \
            -L$$EXTRA_PREFIX/libcddb/lib -lcddb \
            -lm -lwinmm -mwindows -liconv -lws2_32 -lregex
}
