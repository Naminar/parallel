import subprocess
import matplotlib.pyplot as plt
import numpy as np

def run_subprocess(cmd):
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    return int(result.stdout.split(' ')[1])

def run_serial():
    return [run_subprocess(["./serial"]) for n in range(1, 9)]

def run_omp():
    return [run_subprocess(["./omp", str(n)]) for n in range(1, 9)]
        

def make_plot(line_values, bar_values):
    x = np.arange(1, 9)
    fig, ax = plt.subplots()
    ax.bar(x - 0.2, bar_values, width=0.4, label='Serial execution', color='b')
    ax.plot(x + 0.2, line_values, label='Parallel execution', color='r', marker='o')

    ax.set_xticks(x)
    # ax.set_xticklabels(categories)
    ax.set_xlabel('Number of threads')
    ax.set_ylabel('Time')
    ax.set_title('Serial and Parallel execution')
    ax.legend()
    plt.show()

if __name__ == "__main__":
    make_plot(run_omp(), run_serial())