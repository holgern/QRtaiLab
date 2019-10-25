# -------------------------------------------------
# Project created by QtCreator 2009-05-03T09:18:11
# -------------------------------------------------
TARGET = qrtailab
DESTDIR = ../
TEMPLATE = app
target.path = /usr/bin
INSTALLS += target
include( ../qrtailab.config )
CONFIG += QwtPlot \
    QwtWidgets
contains(CONFIG, QRL_OpenGL ) { 
    QT += opengl
    DEFINES += _use_opengl_
}
MOC_DIR = ../moc
OBJECTS_DIR = ../obj
UI_DIR = ../ui
INCLUDEPATH += ../src/qtplotlib/ \
    ../src/
SOURCES += ../src/main.cpp \
    ../src/target_thread.cpp \
    ../src/targets_manager.cpp \
    ../src/scope_window.cpp \
    ../src/scopes_manager.cpp \
    ../src/qrtailab_listener.cpp \
    ../src/qrtailab_core.cpp \
    ../src/parameters_manager.cpp \
    ../src/parameters.cpp \
    ../src/meter_window.cpp \
    ../src/meters_manager.cpp \
    ../src/meter_data.cpp \
    ../src/main_window.cpp \
    ../src/log_window.cpp \
    ../src/logs_manager.cpp \
    ../src/log_data.cpp \
    ../src/led_window.cpp \
    ../src/leds_manager.cpp \
    ../src/alog_data.cpp \
    ../src/qtplotlib/matrix_view.cpp \
    ../src/rtai_scope.cpp \
    ../src/qtplotlib/data2disk.cpp \
    ../src/qtplotlib/scope_trace.cpp \
    ../src/qtplotlib/scope_data.cpp \
    ../src/qtplotlib/scope.cpp \
    ../src/qtplotlib/led.cpp \
    ../src/qtplotlib/led_data.cpp \
    ../src/qtplotlib/thermo_qwt.cpp \
    ../src/qtplotlib/dial_qwt.cpp \
    ../src/qtplotlib/lcd_qlabel.cpp \
    ../src/qtplotlib/matrix_plot.cpp \
    ../src/graphic_scene.cpp \
    ../src/qtplotlib/xyplot.cpp \
    ../src/qtplotlib/xyplot_trace.cpp
HEADERS += ../src/targets_manager.h \
    ../src/target_thread.h \
    ../src/scope_window.h \
    ../src/scopes_manager.h \
    ../src/qrtailab_listener.h \
    ../src/qrtailab.h \
    ../src/qrtailab_core.h \
    ../src/parameters_manager.h \
    ../src/parameters.h \
    ../src/meter_window.h \
    ../src/meters_manager.h \
    ../src/meter_data.h \
    ../src/main_window.h \
    ../src/log_window.h \
    ../src/logs_manager.h \
    ../src/log_data.h \
    ../src/led_window.h \
    ../src/leds_manager.h \
    ../src/alog_data.h \
    ../src/qtplotlib/matrix_view.h \
    ../src/rtai_scope.h \
    ../src/qtplotlib/data2disk.h \
    ../src/qtplotlib/scope_trace.h \
    ../src/qtplotlib/scope_data.h \
    ../src/qtplotlib/scope.h \
    ../src/qtplotlib/qtplot_global.h \
    ../src/qtplotlib/led.h \
    ../src/qtplotlib/led_data.h \
    ../src/qtplotlib/thermo_qwt.h \
    ../src/qtplotlib/dial_qwt.h \
    ../src/qtplotlib/lcd_qlabel.h \
    ../src/qtplotlib/matrix_plot.h \
    ../src/graphic_scene.h \
    ../src/qtplotlib/xyplot.h \
    ../src/qtplotlib/xyplot_trace.h
FORMS += ../src/qrl_leds_manager.ui \
    ../src/qrl_targets_manager.ui \
    ../src/qrl_scopes_manager.ui \
    ../src/qrl_parameters_manager.ui \
    ../src/qrl_meters_manager.ui \
    ../src/qrl_main_window.ui \
    ../src/qrl_logs_manager.ui
RESOURCES += ../src/qrtailab.qrc
