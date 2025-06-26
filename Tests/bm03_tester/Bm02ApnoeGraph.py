import csv

from matplotlib import pyplot as plt


csv_file = 'output_full.csv'

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

fig, ax = plt.subplots(figsize=(8, 4.5))
# ax.set_xlabel("Perioda dechu [sekundy]")
ax.set_xlabel("Počet nádechů za minutu [1/min]")
ax.set_ylabel("Čas do vyhlášení alarmu [sekundy]")
plt.title("Závislost času vyhlášení poplachu mělkého dechu na rychlosti dechu")

y3 = [(7 * i + 8 + 3) if i > 8 else 0 for i in x2]


plt.plot(x1, y1, 'r.')
plt.plot(x1, y2, 'b-')
plt.plot(x1, y3, 'k.')

plt.legend(('SN 5000306770', 'SN 5000305942', 'teoreticky správně'))

# plt.show()

plt.savefig('bm02_melky_dech.png')
