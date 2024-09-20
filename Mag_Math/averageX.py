import csv


# Function to calculate the average X value
def calculate_average_x(input_file):
    total_x = 0
    count = 0

    with open(input_file, 'r') as csvfile:
        reader = csv.DictReader(csvfile)

        for row in reader:
            x_value = row.get('Mag X')
            if x_value:
                try:
                    total_x += float(x_value)
                    count += 1
                except ValueError:
                    print(f"Skipping invalid X value: {x_value}")

    if count == 0:
        return 0

    average_x = total_x / count
    return average_x


# Specify the input file
input_file = 'redo0degoutput.csv'

# Calculate and print the average X value
average_x = calculate_average_x(input_file)
print(f"The average X value is: {average_x}")