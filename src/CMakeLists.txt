cmake_minimum_required(VERSION 3.2)
set($TARGET infinite_matrix)

add_executable(${TARGET}
    main.cpp
    interfaces.h
    matrix_row.h
    infinite_matrix.h
    value_keeper.h)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# максимально строгие настройки компилятора
if (MSVC)
    target_compile_options(${TARGET} PRIVATE
        /W4
    )
    target_compile_options(${TARGET} PRIVATE
        /W4
    )
else ()
    target_compile_options(${TARGET} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(${TARGET} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

# бинарник кладем в каталог bin
install(TARGETS ${TARGET} RUNTIME DESTINATION bin)

# генерить будем deb пакет
set(CPACK_GENERATOR DEB)

# конфигурируем deb пакет
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT oshuev.valery@yandex.ru)

# подключем генератор пакетов
include(CPack)
