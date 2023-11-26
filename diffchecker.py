import difflib

def compare_files(file1_path, file2_path):
    # Read the contents of the files
    with open(file1_path, 'r', encoding='utf-8') as file1:
        file1_content = file1.readlines()
    
    with open(file2_path, 'r', encoding='utf-8') as file2:
        file2_content = file2.readlines()

    # Compute the difference between the two files
    differ = difflib.Differ()
    diff = list(differ.compare(file1_content, file2_content))

    # Print the differences
    for line in diff:
        print(line)

if __name__ == "__main__":
    file1_path = "./output.txt"
    file2_path = "./random_file.txt"

    compare_files(file1_path, file2_path)
