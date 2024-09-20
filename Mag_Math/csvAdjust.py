import csv
import numpy as np

# Define the input and output file paths
input_file = 'calibOutsidePole8CSV.csv'
output_file = 'AAAOUTSIDEPOLE8OUTPUT.csv'

# Define the 3x3 matrix A and 3x1 vector b
A = np.array([[0.523049, -0.034468, -0.115780],
              [-0.034468, 0.477781, -0.095025],
              [-0.115780, -0.095025, 0.361289]])

b = np.array([-530.677828, -3.379828, -737.516695])


def transform_point(x, A, b):
    x_vector = np.array(x)
    transformed_vector = A @ (x_vector - b)
    return transformed_vector


def convert_to_float(value):
    try:
        return float(value)
    except ValueError:
        return None  # Return None if the conversion fails


# Open the input CSV file for reading
with open(input_file, 'r', newline='') as csv_file:
    csv_reader = csv.reader(csv_file)

    # Skip the first line (header or initial data)
    next(csv_reader)

    # Open the output CSV file for writing
    with open(output_file, 'w', newline='') as output_csv_file:
        csv_writer = csv.writer(output_csv_file)

        # Iterate over the remaining rows in the CSV file
        for row in csv_reader:
            # Convert each row element to a float if possible, skip if any conversion fails
            x = [convert_to_float(value) for value in row]

            # Ensure that all converted values are not None (valid floats)
            if None not in x:
                transformed_x = transform_point(x, A, b)
                # Write the transformed vector to the output CSV file
                csv_writer.writerow(transformed_x)
            else:
                print(f"Skipping invalid row: {row}")

print(f"Transformation complete. The transformed data is saved as {output_file}")