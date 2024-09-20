import pandas as pd
import numpy as np


def transform_data(input_csv, output_csv, A, b):
    # Read the input CSV file
    df = pd.read_csv(input_csv)

    # Check if the required columns are present
    if not all(column in df.columns for column in ['Mag X', 'Mag Y', 'Mag Z']):
        raise ValueError("Input CSV must contain 'Mag X', 'Mag Y', and 'Mag Z' columns")

    # Convert the DataFrame to a numpy array
    data = df[['Mag X', 'Mag Y', 'Mag Z']].values

    # Subtract b from each row
    data_minus_b = data - b

    # Multiply by matrix A
    transformed_data = np.dot(data_minus_b, A.T)

    # Create a new DataFrame with the transformed data
    transformed_df = pd.DataFrame(transformed_data, columns=['Transformed X', 'Transformed Y', 'Transformed Z'])

    # Save the transformed data to the output CSV file
    transformed_df.to_csv(output_csv, index=False)


# Example usage
A = np.array([[1.658263, -0.142316, -0.005845],
              [-0.142316, 1.564225, -0.001726],
              [-0.005845, -0.001726, 2.295618]])
b = np.array([20.849303, 38.504897, 165.180596])

input_csv = 'trial13PoleFulloutput.csv'
output_csv = 'trail13PoleFull_adjusted.csv'

transform_data(input_csv, output_csv, A, b)