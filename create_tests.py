import os


def create_test():
    # test_string = "你们好āa"
    # test_string = "Hello world! Test1\n, test2\n, test3"

    # A Unicode code point and a character are not equivalent, for example: Hebrew word Shalom - "שָלוֹם" that
    # consists of 4 characters and 6 code points (4 base letters and 2 diacritical marks)
    # https://www.boost.org/doc/libs/1_48_0/libs/locale/doc/html/boundary_analysys.html
    # test_string = "שָלוֹם"
    test_string = "😂😁"
    # test_string = "☃"
    # test_string = "😂😘❤️😍😊😁👍Hello!☺️😔😄😭💋😒😳😜🙈😉😃😢😝😱😡😏😞😅😚🙊😌😀😋😆👌😐😕"
    print(len(test_string))

    with open(os.path.join("files_with_errors", "task_test_UTF-32_BE_error_small2.txt"),
              "w", encoding="UTF-32-BE") as f:
        f.write(test_string)


if __name__ == '__main__':
    # print(len("😂😘❤️😍😊😁👍Hello!☺️😔😄😭💋😒😳😜🙈😉😃😢😝😱😡😏😞😅😚🙊😌😀😋😆👌😐😕"))
    create_test()
    # test_file_path = "files_with_errors"
    # print(os.getcwd() + test_file_path)
    # print(os.path.isdir(os.getcwd() + "/" + test_file_path))
