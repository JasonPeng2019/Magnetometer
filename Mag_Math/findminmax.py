import csv


def find_min_max(file_path, column_name):
    with open(file_path, 'r') as file:
        reader = csv.DictReader(file)
        numbers = []

        for row in reader:
            try:
                num = float(row[column_name].strip())
                numbers.append(num)
            except ValueError:
                continue  # Skip rows that cannot be converted to a float

        if not numbers:
            raise ValueError("No valid numbers found in the specified column.")

        min_value = min(numbers)
        max_value = max(numbers)

        # Find the minimum positive number
        positive_numbers = [num for num in numbers if num > 0]
        if positive_numbers:
            min_positive_value = min(positive_numbers)
        else:
            min_positive_value = None  # No positive numbers found

    return min_value, max_value, min_positive_value


# Example usage:
file_path = 'redofullset1output.csv'  # Change this to the path of your CSV file
column_name = 'Mag X'  # Change this to the column you want to process
min_value, max_value, min_positive_value = find_min_max(file_path, column_name)
print(f"Minimum value: {min_value}")
print(f"Maximum value: {max_value}")
if min_positive_value is not None:
    print(f"Minimum positive value: {min_positive_value}")
else:
    print("No positive numbers found.")