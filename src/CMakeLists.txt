set(kturtle_SRCS
    main.cpp
    interpreter/echoer.cpp
    interpreter/errormsg.cpp
    interpreter/executer.cpp
    interpreter/interpreter.cpp
    interpreter/parser.cpp
    interpreter/token.cpp
    interpreter/tokenizer.cpp
    interpreter/translator.cpp
    interpreter/treenode.cpp
    interpreter/value.cpp
)

set(CMAKE_AUTOMOC ON)

add_executable(bacs.kturtle ${kturtle_SRCS}
                       ${kturtle_RCC_SRCS}
)

target_link_libraries(bacs.kturtle
    Qt5::Core
    Qt5::Xml
)

install (TARGETS  bacs.kturtle RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#install (PROGRAMS    org.kde.kturtle.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
#install (FILES    kturtleui.rc     DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kturtle)
#install (FILES    kturtle.knsrc    DESTINATION ${KDE_INSTALL_CONFDIR})
#
