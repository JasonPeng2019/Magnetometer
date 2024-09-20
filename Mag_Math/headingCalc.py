import pandas as pd
import numpy as np

# Read the input CSV file
input_file = 'trial13_for_heading.csv'  # Replace with your input file name
output_file = 'trial13_adjusted_heading.csv'  # Replace with your desired output file name

# Load the CSV file into a DataFrame
df = pd.read_csv(input_file)

# Function to calculate heading in degrees and normalize to [-180, 180)
def calculate_heading(row):
    if pd.notnull(row['Mag X']) and pd.notnull(row['Mag Z']):
        heading = np.degrees(np.arctan2(row['Mag Z'], row['Mag X']))
        return heading
    else:
        return np.nan

# Apply the function to each row to calculate the heading
df['heading'] = df.apply(calculate_heading, axis=1)

# Save the DataFrame to a new CSV file
df.to_csv(output_file, index=False)

print(f"Output written to {output_file}")