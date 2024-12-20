import subprocess
import matplotlib.pyplot as plt
import numpy as np

def run_subprocess(cmd):
    print(cmd)
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    return int(result.stdout.split(' ')[1])

def run_serial():
    return np.array([run_subprocess(["./serial"]) for n in range(2, 7)])

def run_mpi():
    return np.array([run_subprocess(['/usr/bin/mpiexec', '-np', str(n), '--oversubscribe',  './mpi']) for n in range(2, 7)])

def run_mpi_baseline():
    return np.array([run_subprocess(['/usr/bin/mpiexec', '-np', str(n), '--oversubscribe',  './baseline']) for n in [2, 2, 4, 4, 4]])

def make_plot(line_values, line_values_dep, bar_values):
    x = np.arange(2, 7)
    fig, ax = plt.subplots()
    ax.bar(x - 0.2, bar_values, width=0.4, label='Serial execution', color='b')
    ax.plot(x + 0.2, line_values, label='Parallel execution', color='r', marker='o')
    ax.plot(x + 0.2, line_values_dep, label='Parallel execution no dependency', color='y', marker='o')

    ax.set_xticks(x)
    # ax.set_xticklabels(categories)
    ax.set_xlabel('Number of processes')
    ax.set_ylabel('Time')
    ax.set_title('Serial and Parallel execution')
    ax.legend()
    plt.show()

def make_plot_speedup(line_values, line_values_dep, bar_values):
    x = np.arange(2, 7)
    fig, ax = plt.subplots()

    serial_norm = bar_values.sum()/len(bar_values)

    # ax.bar(x - 0.2, bar_values, width=0.4, label='Serial execution', color='b')
    ax.plot(x + 0.2, serial_norm/line_values, label='Parallel execution with dependency', color='r', marker='o')
    ax.plot(x + 0.2, serial_norm/line_values_dep, label='Parallel execution no dependency', color='y', marker='o')

    ax.set_xticks(x)
    # ax.set_xticklabels(categories)
    ax.set_xlabel('Number of threads')
    ax.set_ylabel('Speedup')
    ax.set_title('Serial and Parallel execution')
    ax.legend()
    plt.show()

if __name__ == "__main__":
    # make_plot(run_mpi(), run_serial())
    mpi, mpi_baseline, serial = run_mpi(), run_mpi_baseline(), run_serial()
    make_plot(mpi, mpi_baseline, serial)
    make_plot_speedup(mpi, mpi_baseline, serial)
