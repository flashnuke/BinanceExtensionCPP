import pandas as pd

results = {
    "e_timestamps": [],
    "t_timestamps": [],
    "l_timestamps": []
}

with open("results.txt", "r") as file:
    for x in file:
        e, t, l = x.split(",")
        results["e_timestamps"].append(int(e))
        results["t_timestamps"].append(int(t))
        results["l_timestamps"].append(int(l))

df = pd.DataFrame(data=results)
df.to_csv("results.csv", index=False)
