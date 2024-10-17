cmake_minimum_required(VERSION 3.26)
project(t2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -Wall")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(base)
include_directories(app/OrderListWindow)
include_directories(app/OrderListWindow/widgets)
include_directories(app/ProductManager)

add_executable(t2 main.cpp
        base/Order.cpp
        base/Product.cpp
        base/Invoice.cpp
        app/OrderListWindow/orderlistwindow.cpp
        app/OrderListWindow/orderlistwindow.ui
        app/OrderListWindow/widgets/OrderListTable.cpp
        app/OrderListWindow/widgets/OrderListTable.h
        app/OrderListWindow/widgets/ProductListTable.h
        app/OrderListWindow/widgets/ProductListTable.cpp
        app/OrderListWindow/widgets/InvoiceTable.cpp
        app/OrderListWindow/widgets/InvoiceTable.h
        base/PriceList.cpp
        app/ProductManager/productmanager.cpp
        app/ProductManager/productmanager.h
        app/ProductManager/productmanager.ui
        app/ProductManager/ProductTable.cpp
        app/ProductManager/ProductTable.h
        app/OrderListWindow/OrderLoader.cpp
        app/OrderListWindow/OrderLoader.h)

target_link_libraries(t2
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

