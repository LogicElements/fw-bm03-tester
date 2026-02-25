import csv

from matplotlib import pyplot as plt


csv_file = 'output_full_1.csv'
csv_file_2 = 'output_full_2.csv'

x1 = []
x2 = []
y1 = []
y2 = []

with open(csv_file, 'r', newline='') as f:
    reader = csv.reader(f)
    for row in reader:
        try:
            x1.append(round(float(row[0]), 2))
            x2.append(round(float(row[1]), 2))
            y1.append(round(float(row[2]), 2))
            y2.append(round(float(row[3]), 2))
        except ValueError:
            pass

y4 = []
y5 = []

with open(csv_file_2, 'r', newline='') as f:
    reader = csv.reader(f)
    for row in reader:
        try:
            y4.append(round(float(row[2]), 2))
            y5.append(round(float(row[3]), 2))
        except ValueError:
            pass

fig, ax = plt.subplots(figsize=(8, 4.5))
# ax.set_xlabel("Perioda dechu [sekundy]")
ax.set_xlabel("Počet nádechů za minutu [1/min]")
ax.set_ylabel("Čas do vyhlášení alarmu [sekundy]")
plt.title("Závislost času vyhlášení poplachu mělkého dechu na rychlosti dechu")

y3 = [18+3 if i > 18 else (7 * i + 8 + 3) if i > 8 else 0 for i in x2]


plt.plot(x1, y1, 'r-')
plt.plot(x1, y2, 'b-')
plt.plot(x1, y3, 'k-')
plt.plot(x1, y4, 'g-')
plt.plot(x1, y5, 'y-')

plt.legend(('305942 WDT 20.58', '306770 WDT 20.43', 'výpočet WDT 18.0', '302374 WDT 17.60', '307091 WDT 16.02'))

# plt.show()

plt.savefig('bm02_melky_dech.png')
