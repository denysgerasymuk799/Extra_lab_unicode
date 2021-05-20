# Extra_lab_unicode

- можна і так //    fin.imbue(std::locale(fin.getloc(),
  //                          new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
  
- utf- що опрацьовувало всі файли з назвою що включає цю substring
-  строго має бути таким чином написано бо encoding=config_file["encoding"]
- якщо у config.json не вказати config.json то опрацює всі файли у вкаазній директорії "test_files_dir_path"
- "encoding": "utf-32-BE" чисто для python
- назва файлу через дефіс
- encoding обовязково вказувати у конфігу для пайтона


# Запуск

- сюди додавати ../config.dat бо read_config("../config.dat", config_params);


## Тести

-  print(len("😂😘❤️😍😊😁👍Hello!☺️😔😄😭💋😒😳😜🙈😉😃😢😝😱😡😏😞😅😚🙊😌😀😋😆👌😐😕")) // 42 
-  print(len("Hello world! Test1\n, test2\n, test3")) // 34
- small2 -- 6 characters

- приклад для пояснення правильності підрахунку код юнітів -- ../files/task_test_UTF-16_small2.txt: 6


- пояснити чому у тому що я прінчу то там помилки -- типу непарна кількість байт у utf-16 файлі