

for i in range(0, len(listforce)):
    print (listforce[i][1])

fig, ax = plt.subplots()
ax.plot(particleXaxis, listparticleId, label='--', linestyle='-', marker='o', markersize=10, markevery=1)

print(listmassB)

plt.xlabel("time")
plt.ylabel("unit")
legend = plt.legend(loc='best', shadow=True)
plt.title('----')
plt.show()
