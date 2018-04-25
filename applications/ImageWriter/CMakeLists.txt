add_executable(image_writer main.cpp)
target_link_libraries(image_writer pathviz)
target_link_libraries(image_writer ${PATHVIZ_LIBRARIES})