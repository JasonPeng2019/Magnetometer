import os
import csv

# Function to process a single file
def process_file(input_file, output_file):
    print(f"writing {input_file}....")

    with open(input_file, 'r') as file:
        lines = file.readlines()

    # Initialize variables
    data_entries = []

    # Process the lines
    current_entry = {}
    for line in lines:
        line = line.strip()
        if line:
            try:
                section, value = line.split(': ', 1)
                current_entry[section] = value
            except ValueError:
                print(f"Skipping line: {line}")
        else:
            # If the line is empty and current_entry is not empty, save it
            if current_entry:
                data_entries.append(current_entry)
                current_entry = {}

    # Append the last entry if not already added
    if current_entry:
        data_entries.append(current_entry)

    # Write to CSV
    with open(output_file, 'w', newline='') as csvfile:
        fieldnames = ['Mag X', 'Mag Y', 'Mag Z']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()
        for entry in data_entries:
            row = {
                'Mag X': entry.get('Mag X', ''),
                'Mag Y': entry.get('Mag Y', ''),
                'Mag Z': entry.get('Mag Z', '')
            }
            writer.writerow(row)

    print(f"Data has been written to {output_file}")

# Get all files in the current directory that start with "engr"
input_files = [file for file in os.listdir('.') if file.startswith('engr') and file.endswith('.txt')]

# Process each file
for input_file in input_files:
    output_file = input_file.replace('.txt', '_output.csv')
    print(f"Data writing to {output_file}")
    process_file(input_file, output_file)