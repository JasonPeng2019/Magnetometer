import csv
#a = "14Full"
# Read the data from the text file
#input_file = 'dataTrial'+str(a)+'.txt'
#output_file = 'trial'+str(a)+'output.csv'
input_file = 'Trial1NewCal_data.txt'
output_file = 'Trial1_New_Data.txt'

with open(input_file, 'r') as file:
    lines = file.readlines()

# Initialize variables
data_entries = []

# Process the lines
current_entry = {}
for line in lines:
    line = line.strip()
    if line:
        section, value = line.split(': ')
        current_entry[section] = value
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