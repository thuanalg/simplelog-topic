clang-format -i include/simplelog.h
clang-format -i src/simplelog.c
#clang-format -i tests/console/main.c
#clang-format -i tests/gtk/gtk_app.c

python3 ~/x/line.py include/simplelog.h
#python3 ~/x/line.py src/simplelog.c
#python ~/x/line.py tests/console/main.c
#python ~/x/line.py tests/gtk/gtk_app.c
