import os
import time


class Logger:
    def __init__(self, debug=False, append=None, custom_run_name=None):
        self.debug = debug
        if self.debug:
            return

        output_folder = './output/'
        run_folder = 'run%Y%m%d-%H%M%S' if custom_run_name is None else custom_run_name
        if not os.path.exists(output_folder):
            os.makedirs(output_folder)
        self.path = ''.join([output_folder, time.strftime(run_folder)])
        if append is not None:
            self.path += '_' + str(append)
        self.path += '/'
        if not os.path.exists(self.path):
            os.makedirs(self.path)

        self.save_file = self.path + 'model.h5'

    def log(self, data):
        if not self.debug:
            try:
                logfile = open(self.path + 'log.txt', 'a')
            except IOError:
                print('Logger:log IO error while opening log file %s' % self.path + 'log.txt')
                return
            if type(data) is dict:
                for k in data:
                    logfile.write(k + ': ' + str(data[k]) + '\n')
                    print(k + ': ' + str(data[k]))
            elif type(data) is tuple:
                logfile.write(data[0] + ': ' + str(data[1]) + '\n')
            elif type(data) is str:
                logfile.write(data + '\n')
            elif type(data) is list:
                logfile.write(','.join(str(elem) for elem in data) + '\n')
        print(data)

    def to_csv(self, filename, row):
        if not self.debug:
            try:
                f = open(self.path + filename, 'a')
            except IOError:
                print('Logger:to_csv IO error while opening file %s' % \
                      self.path + filename)
                return
            if type(row) is list:
                string = ','.join([str(val) for val in row])
            elif type(row) is str:
                string = row
            else:
                string = str(row)  # Try to convert it anyway
            string = string + '\n' if not string.endswith('\n') else ''
            f.write(string)
            f.close()
