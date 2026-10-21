import csv, matplotlib; matplotlib.use("Agg"); import matplotlib.pyplot as plt
rows = [(int(a), float(b)) for a, b in csv.reader(open("results.csv"))]
th = [r[0] for r in rows]; tm = [r[1] for r in rows]; sp = [tm[0] / x for x in tm]
plt.figure(); plt.plot(th, tm, "o-"); plt.xlabel("No of Threads"); plt.ylabel("Time (s)"); plt.title("Threads vs Time"); plt.grid(); plt.savefig("threads_vs_time.png", dpi=150)
plt.figure(); plt.plot(th, sp, "o-"); plt.xlabel("No of Threads"); plt.ylabel("Speedup"); plt.title("Threads vs Speedup"); plt.grid(); plt.savefig("threads_vs_speedup.png", dpi=150)
