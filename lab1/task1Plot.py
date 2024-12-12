import subprocess
import matplotlib.pyplot as plt
import numpy as np

def run_subprocess(cmd):
    print(cmd)
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    return int(result.stdout.split(' ')[1])

def run_serial():
    return [run_subprocess(["./serial"]) for n in range(2, 7)]

def run_mpi():
    return [run_subprocess(['/usr/bin/mpiexec', '-np', str(n), '--oversubscribe',  './mpi']) for n in range(2, 7)]
        

def make_plot(line_values, bar_values):
    # plt.plot(data)
    # plt.show()
    # categories = ['A', 'B', 'C', 'D']
    # bar_values = [3, 12, 5, 18]
    # line_values = [1, 10, 6, 15]
    x = np.arange(len(line_values))
    fig, ax = plt.subplots()
    ax.bar(x - 0.2, bar_values, width=0.4, label='Bar Values', color='b')
    ax.plot(x + 0.2, line_values, label='Line Values', color='r', marker='o')

    ax.set_xticks(x)
    # ax.set_xticklabels(categories)
    ax.set_xlabel('Categories')
    ax.set_ylabel('Values')
    ax.set_title('Combined Bar and Line Plot')
    ax.legend()
    plt.show()

if __name__ == "__main__":
    # run_serial()
    # run_omp()
    # print(subprocess.run(['/usr/bin/mpiexec -np 2  ./mpi;'], capture_output=True, text=True).stdout)
    make_plot(run_mpi(), run_serial())
#     import subprocess

# # Correctly pass the command as a list of arguments
#     result = subprocess.run(['/usr/bin/mpiexec', '-np', '2', './mpi'], capture_output=True, text=True)

# # Print the output
#     print(result.stdout)