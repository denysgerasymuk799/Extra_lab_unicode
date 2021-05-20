import os
import sys
import json
import platform


DEBUG_MODE = False


def check_results(result_file, config_file):
    all_correct = True

    with open(config_file["path_expected_results"], "r", encoding="utf-8") as f:
        expected_results_dict = json.load(f)

    for i, line in enumerate(result_file.readlines()):
        result = int(line.split()[1])
        file_path = line.split()[0][:-1]
        if result != expected_results_dict[file_path]:
            all_correct = False
            print(
                f"Incorrect result for file {file_path} result -- {result},"
                f" but expected_result -- {expected_results_dict[file_path]}"
            )

    if all_correct:
        print(f"All results correct. Check results_task{config_file['task']}.txt and"
              f" expected_results_task{config_file['task']}.json")

    else:
        print(f"Check results_task{config_file['task']}.txt and"
              f" expected_results_task{config_file['task']}.txt")


def check_results_task3(result_file, config_file):
    with open(config_file["path_expected_results"], "r", encoding="utf-8") as f:
        expected_results = f.read()

    if result_file == expected_results:
        print(f"All results correct. Check results_task{config_file['task']}.txt and"
              f" expected_results_task{config_file['task']}.txt")

    else:
        print(f"Check results_task{config_file['task']}.txt and"
              f" expected_results_task{config_file['task']}.txt")


def run_all_experiments(cpp_filename, config_file):
    executed_from = "from_python"
    encoding = "-"
    if config_file["encoding"].find("utf-") != -1:
        encoding = config_file["encoding"]

    extension_pos = config_file["path_save_results"].rfind(".")
    config_file["path_save_results"] = config_file["path_save_results"][:extension_pos] + \
                                       "_task" + config_file["task"] + \
                                       config_file["path_save_results"][extension_pos:]

    first_file = False
    for test_file_path in os.listdir(config_file["test_files_dir_path"]):
        if config_file["encoding"] != "-" and len(config_file["encoding"]) > 1 \
                and test_file_path.lower().find(config_file["encoding"].lower()) == -1:
            continue

        test_file_path = os.path.join(config_file["test_files_dir_path"], test_file_path)
        command_line = " ".join([cpp_filename,
                                 config_file["task"], encoding,
                                 test_file_path, config_file["path_save_results"], executed_from])

        if not first_file:
            command_line += "_start"
            first_file = True

        if DEBUG_MODE:
            print("\n\ncommand_line -- ", command_line)

        os.system(command_line)


if __name__ == '__main__':
    argc = len(sys.argv)
    argv = sys.argv

    if not os.path.exists(argv[0]):
        print("You should input valid path of a config file")
        exit(-1)

    if argc == 1:
        config_path = os.path.join("..", "configs", "config.json")
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

        run_all_experiments(cpp_filename, config_file)

        if config_file["task"] in ("1", "2"):
            with open(config_file["path_save_results"], "r", encoding="utf-8") as result_file:
                check_results(result_file, config_file)

        elif config_file["task"] == "3":
            with open(config_file["path_save_results"], "r", encoding="utf-8") as f:
                result_file = f.read()
                check_results_task3(result_file, config_file)

    else:
        print('Binary file does not exist')
