import numpy as np
import csv


def read_tsv(file_path):
    # Read TSV file and convert to a numpy array, handling non-numeric values
    with open(file_path, 'r') as file:
        reader = csv.reader(file, delimiter='\t')
        data = []
        for row in reader:
            # Convert each value to float, replacing non-numeric with NaN
            cleaned_row = [float(val) if val else np.nan for val in row]
            data.append(cleaned_row)
        return np.array(data)


def calculate_A_x_minus_b(A, b, X):
    # Convert b to a column vector if it's not already
    b = np.reshape(b, (-1, 1))

    # Compute A(x - b) for each data point x in X, ignoring rows with NaN values
    results = []
    for x in X:
        if np.isnan(x).any():
            continue  # Skip rows with NaN values
        x = np.reshape(x, (-1, 1))
        result = A @ (x - b)
        results.append(result.flatten())  # Flatten the result to a 1D array

    return np.array(results)


def write_csv(file_path, data):
    # Write the numpy array data to a CSV file
    with open(file_path, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerows(data)


# Define the matrices A and b
A = np.array([[1.070962, -0.091913, -0.003775],
              [-0.091913, 1.010229, -0.001115],
              [-0.003775, -0.001115, 1.482587]])
b = np.array([20.849303, 38.504897, 165.180596])


# Read the data points X from a TSV file
input_file_path = 'trial13Poleoutput.txt'  # Change this to the path of your TSV file
X = read_tsv(input_file_path)

# Calculate A(x - b) for each data point x in X
results = calculate_A_x_minus_b(A, b, X)

# Write the results to a CSV file
output_file_path = 'trial13_adjusted.csv'  # Change this to the desired output file path
write_csv(output_file_path, results)