#g++ -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o build/implot_items.o ../../implot_items.cpp
#g++ -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o build/implot.o ../../implot.cpp
#g++ -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o build/implot_demo.o ../../implot_demo.cpp
g++ -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -c -o build/main.o main.cpp
g++ -o std_cubetimer build/main.o build/implot.o build/implot_items.o build/implot_demo.o build/imgui.o build/imgui_demo.o build/imgui_draw.o build/imgui_tables.o build/imgui_widgets.o build/imgui_impl_glfw.o build/imgui_impl_opengl3.o -std=c++20 -I../.. -I../../backends -g -Wall -Wformat `pkg-config --cflags glfw3` -lGL `pkg-config --static --libs glfw3`
./std_cubetimer