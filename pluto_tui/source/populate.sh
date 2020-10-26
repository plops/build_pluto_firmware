cd ~/src

git clone https://github.com/ggerganov/imtui --recursive

for i in third-party/imgui/imgui/imgui.cpp \
third-party/imgui/imgui/imgui_draw.cpp \
third-party/imgui/imgui/imgui_demo.cpp \
third-party/imgui/imgui/imgui_widgets.cpp \
src/imtui-impl-text.cpp \
src/imtui-impl-ncurses.cpp \
		    examples/imtui-demo.cpp ; do
    cp /home/martin/src/imtui/$i .
done
