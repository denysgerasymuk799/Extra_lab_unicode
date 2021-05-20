import os
import sys
import json
import platform


def check_results(result_file, config_file):
    expected_results_string = ""
    all_correct = True

    with open(config_file["path_expected_results"], "r", encoding="utf-8") as f:
        expected_results_dict = json.load(f)

    for i, line in enumerate(result_file.readlines()):
        result = int(line.split()[1])
        file_path = line.split()[0][:-1]
        if result != expected_results_dict[file_path]:
            all_correct = False
            print(f"Incorrect result for file {file_path} result -- {result}, but expected_result -- {expected_results_dict[file_path]}")

        expected_results_string += file_path + " " + str(expected_results[i]) + "\n"

    # with open(os.path.join("..", "results", "expected_results_task" + config_file["task"] + ".txt"),
    #           "w", encoding="utf-8") as f:
    #     f.write(expected_results_string)

    if all_correct:
        print(f"All results correct. Check results_task{config_file['task']}.txt and"
              f" expected_results_task{config_file['task']}.txt")


def run_all_experiments(cpp_filename, config_file):
    executed_from = "from_python"
    # test_file_path = "../files/task1_test_UTF32_big.txt"
    # endianness = "-"
    encoding = "-"
    print('config_file["encoding"].find("utf-") -- ', config_file["encoding"].find("utf-"))
    if config_file["encoding"].find("utf-") != -1:
        encoding = config_file["encoding"]

    extension_pos = config_file["path_save_results"].rfind(".")
    config_file["path_save_results"] = config_file["path_save_results"][:extension_pos] + "_task" + config_file["task"] + config_file["path_save_results"][extension_pos:]
    # if config_file["endianness"] in ("BE", "LE"):
    #     endianness = config_file["endianness"]

    first_file = False
    expected_results = []
    for test_file_path in os.listdir(config_file["test_files_dir_path"]):
        # pos = config_file["encoding"].find("BE")
        # substr = config_file["encoding"][:pos] + config_file["encoding"][pos + 4:]
        if config_file["encoding"] != "-" and len(config_file["encoding"]) > 1\
                and test_file_path.lower().find(config_file["encoding"].lower()) == -1:
            continue

        test_file_path = os.path.join(config_file["test_files_dir_path"], test_file_path)
        command_line = " ".join([cpp_filename,
                                 config_file["task"], encoding,
                                 test_file_path, config_file["path_save_results"], executed_from])

        if not first_file:
            command_line += "_start"
            first_file = True

        print("\n\ncommand_line -- ", command_line)
        os.system(command_line)

        if encoding.lower().find("utf") == -1:
            encodings = ["UTF-32", "UTF-32-BE",
                         "UTF-16", "UTF-16-BE",
                         "UTF-8", "UTF-8-BE"]

            found_encoding = False
            for enc in encodings:
                try:
                    with open(test_file_path, "r", encoding=enc) as f:
                        expected_results.append(len(f.read()))

                    found_encoding = True
                    print("test_file_path -- ", test_file_path)
                    print("found_encoding -- ", enc)
                    break
                except:
                    pass

            if not found_encoding:
                expected_results.append(0)

        else:
            try:
                try:
                    with open(test_file_path, "r", encoding=encoding) as f:
                        expected_results.append(len(f.read()))

                except UnicodeDecodeError:
                    with open(test_file_path, "r", encoding=encoding + "-BE") as f:
                        expected_results.append(len(f.read()))

            except:
                expected_results.append(0)

    return expected_results


if __name__ == '__main__':
    # create_test()
    # python run_tests.py ./config.json
    argc = len(sys.argv)
    argv = sys.argv
    print("argc -- ", argc)
    print("argv -- ", argv)

    if not os.path.exists(argv[0]):
        print("You should input valid path of a config file")
        exit(-1)

    if argc == 1:
        config_path = "../config.json"
    else:
        config_path = argv[1]

    cpp_filename = "lab_unicode"
    if platform.system() == "Windows":
        cpp_filename += ".exe"

    elif platform.system() == "Linux":
        cpp_filename = "./" + cpp_filename

    if os.path.isfile(cpp_filename):
        print('Binary file already exists')

        with open(config_path, "r", encoding="utf-8") as f:
            config_file = json.load(f)

        expected_results = run_all_experiments(cpp_filename, config_file)

        with open(config_file["path_save_results"], "r", encoding="utf-8") as result_file:
            check_results(result_file, config_file)

    else:
        print('Binary file does not exist')
