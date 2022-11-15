g++ -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o main.o main.cpp
g++ -o std_cubetimer main.o imgui.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_impl_glfw.o imgui_impl_opengl3.o -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -lGL `pkg-config --static --libs glfw3`
./std_cubetimer