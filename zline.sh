clang-format -i include/simplelog.h
clang-format -i include/simplelog_bin_parser.h
clang-format -i src/simplelog_bin_parser.c
clang-format -i src/simplelog.c
clang-format -i tests/c_test/main.c
clang-format -i tests/cpp_test/main.cpp
#clang-format -i tests/gtk/gtk_app.c

python3 ~/x/line.py include/simplelog.h
python3 ~/x/line.py include/simplelog_bin_parser.h
python3 ~/x/line.py src/simplelog.c
python3 ~/x/line.py src/simplelog_bin_parser.c
python ~/x/line.py tests/c_test/main.c
python ~/x/line.py tests/cpp_test/main.cpp

#python ~/x/line.py tests/gtk/gtk_app.c
