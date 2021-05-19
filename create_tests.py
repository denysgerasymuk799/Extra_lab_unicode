if __name__ == '__main__':
    # test_string = "你们好āa"
    test_string = "😂😘❤️😍😊😁👍☺️😔😄😭💋😒😳😜🙈😉😃😢😝😱😡😏😞😅😚🙊😌😀😋😆👌😐😕"

    with open("task1_test_UTF16_big.txt", "w", encoding="utf-16") as f:
        f.write(test_string)
