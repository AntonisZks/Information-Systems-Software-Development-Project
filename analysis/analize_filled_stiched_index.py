import numpy as np
import matplotlib.pyplot as plt

def read_recalls(file_path):
    with open(file_path, "r") as file:
        recalls = [float(line.split(":")[1].split("%")[0]) for line in file.readlines()]
    return recalls

def plot_histogram(recalls, title, file_path):
    plt.figure(figsize=(10, 7))
    plt.hist(recalls, edgecolor='black', bins=10)
    plt.title(title)
    plt.xlabel("Recall")
    plt.ylabel("Frequency")
    plt.tight_layout()
    plt.savefig(file_path)

def plot_pie_chart(recalls, title, file_path):
    ranges = ['0 - 10', '10 - 20', '20 - 30', '30 - 40', '40 - 50', '50 - 60', '60 - 70', '70 - 80', '80 - 90', '90 - 100']
    range_counts = [sum(1 for recall in recalls if start <= recall < end) for start, end in zip(range(0, 100, 10), range(10, 110, 10))]
    filtered_ranges_counts = [(ranges[i], range_counts[i]) for i in range(len(ranges)) if range_counts[i] > 0]
    filtered_ranges, filtered_counts = zip(*filtered_ranges_counts)
    colors = plt.cm.RdYlGn(np.linspace(0, 1, len(filtered_ranges)))
    
    plt.figure(figsize=(10, 7))
    plt.pie(filtered_counts, labels=filtered_ranges, autopct='%1.1f%%', startangle=140, colors=colors)
    plt.title(title)
    plt.legend(filtered_ranges, title="Recall Ranges", loc="center left", bbox_to_anchor=(1, 0, 0.5, 1))
    plt.tight_layout()
    plt.savefig(file_path, bbox_inches='tight')

def main():
    unfiltered_recalls = read_recalls('results/filled/filled_stiched_index_unfiltered_recalls.txt')
    unfiltered_recalls = [recall for recall in unfiltered_recalls if not np.isnan(recall)]
    print("Unfiltered Recall mean: ", np.mean(unfiltered_recalls))
    plot_histogram(
        unfiltered_recalls, 
        "Filled Initialized Stiched Index: Unfiltered Recalls", 
        "analysis/plots/filled/unfiltered/filled_index_unfiltered_recalls_hist.png"
    )
    plot_pie_chart(
        unfiltered_recalls, 
        "Filled Initialized Stiched Index: Unfiltered Recalls", 
        "analysis/plots/filled/unfiltered/filled_index_unfiltered_recalls_pie.png"
    )
    
    filtered_recalls = read_recalls('results/filled/filled_stiched_index_filtered_recalls.txt')
    filtered_recalls = [recall for recall in filtered_recalls if not np.isnan(recall)]
    print("Filtered Recall mean: ", np.mean(filtered_recalls))
    plot_histogram(
        filtered_recalls, 
        "Filled Initialized Stiched Index: Filtered Recalls", 
        "analysis/plots/filled/filtered/filled_index_filtered_recalls_hist.png"
    )
    plot_pie_chart(
        filtered_recalls, 
        "Filled Initialized Stiched Index: Filtered Recalls", 
        "analysis/plots/filled/filtered/filled_index_filtered_recalls_pie.png"
    )

if __name__ == "__main__":
    main()
