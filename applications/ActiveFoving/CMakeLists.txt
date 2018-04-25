add_executable(active_foving main.cpp)
target_link_libraries(active_foving pathviz)
target_link_libraries(active_foving ${PATHVIZ_LIBRARIES})
