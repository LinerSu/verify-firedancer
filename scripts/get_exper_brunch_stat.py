import xml.etree.ElementTree as ET
import re
import csv
import argparse
import yaml
import glob, os
from collections import defaultdict

METRICS_FILE = "metrics.yaml"

def read_brunchstat_from_log(log_file_name):
    with open(METRICS_FILE, 'r') as mtyml:
        metrics = yaml.safe_load(mtyml)
    log_file = open(log_file_name, 'r')
    line = log_file.readline()
    data = list() # list of metric_name->metric
    cur_test = None
    while line:
        # look for next test
        new_test = re.search("Testing: ", line)
        if new_test:
            new_test_data = defaultdict(lambda: 'n/a')
            span = new_test.span()
            test_name = line[span[1]:]
            # remove _unsat_test
            cur_test = test_name[:-12]
            new_test_data['job_name'] = cur_test
            # new_test_data['pp_loc'] = get_loc_info(cur_test, True)
            # new_test_data['opsem_loc'] = get_loc_info(cur_test, False)
            data.append(new_test_data)
        elif line.startswith("BRUNCH_STAT"):
            stat = line.split()
            stat_name = " ".join(stat[1:-1])
            if stat_name in metrics:
                stat_num = stat[-1]
                data[-1][stat_name] = stat_num
        line = log_file.readline()

    log_file.close()
    return data

def get_loc_info(file_name, is_pp):
    # Directory where the files are located
    directory_path = "/tmp/firedancer/crab"

    ir_name = f"{file_name}.ir.fat.pp.ms.bc" if is_pp else f"{file_name}.ir.fat.pp.ms.crab.o.ul.cut.o.bc"

    # Construct the full path to the file with its pattern
    full_path = os.path.join(directory_path, ir_name)
    
    # Get the LOC of the file
    LOC = 'n/a'
    try:
        with open(full_path, 'r') as file:
            lines = file.readlines()
            start = False
            for line in lines:
                if start:
                    LOC += 1
                if '@main' in line:
                    LOC = 0
                    start = True
        return LOC
    except FileNotFoundError:
        return LOC

def read_symbiotic_bruchstat_from_log(log_file_name, xml_file_dir, time_out):
    log_file = open(log_file_name, 'r')
    line = log_file.readline()
    data = defaultdict(list)
    cur_test = None
    while line:
        # look for next test
        new_test = re.search("Test: ", line)
        if new_test:
            span = new_test.span()
            test_name = line[span[1]:]
            # remove _unsat_test
            cur_test = test_name.replace(
                " ", "").replace("\t", "").replace("\n", "")
        elif line.startswith("RESULT:"):
            stat = line.split(':')
            # stat_name = " ".join(stat[1:-1])
            stat_res = stat[-1].replace(
                " ", "").replace("\t", "").replace("\n", "")
            if stat_res == "true":
                stat_res = "passed"
            elif stat_res.startswith("ERROR"):
                stat_res = "failed"
            else:  # unknown
                stat_res = "unknown"
            data[cur_test].append(stat_res)
        elif line.startswith("INFO: Total time elapsed:"):
            time = line.split(':')
            time_res = round(float(time[-1]), 6)
            data[cur_test].append(time_res)
        line = log_file.readline()

    log_file.close()

    for cur_test in data.keys():
        data[cur_test][0], data[cur_test][1] = data[cur_test][1], data[cur_test][0]

    # collect timeout results
    xml_file = glob.glob(
        "{path}/**/Test.xml".format(path=xml_file_dir), recursive=True)[0]
    tree = ET.parse(xml_file)
    root = tree.getroot()
    testlist = root.find('Testing')
    for test in testlist.findall('Test'):
        bench_name = test.find('Name').text
        res = test.get('Status')
        for name_measure_tag in test.find('Results').findall(
                'NamedMeasurement'):
            if name_measure_tag.attrib['name'] == 'Execution Time':
                timing = name_measure_tag.find('Value').text
        if bench_name not in data and float(timing) >= time_out:
            data[bench_name] = [timing, res]

    return data


def write_symbiotic_bruchstat_into_csv(data, out_file):
    with open(out_file, 'w+', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow([
            "Name",
            "Timing",
            "Result"])
        for k, v in data.items():
            writer.writerow([k, *v])


def write_brunchstat_into_csv(data, out_file):
    with open(METRICS_FILE, 'r') as mtyml:
        metrics = yaml.safe_load(mtyml)
        metric_serlz = [(k, v) for k, v in metrics.items()] # ensure ordering
    with open(out_file, 'w+', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow((v for _, v in metric_serlz))
        for job_data in data:
            row = (job_data[k] for k, _ in metric_serlz)
            writer.writerow(row)
    print(f'[Statistics] Please find stat csv file at {out_file}')


# def read_timimg_results(result_dir):
#     log_file = open(log_file_name, 'r')
#     line = log_file.readline()
#     data = defaultdict(list)
#     row_dict = {}
#     while line:
#         if "************** ANALYSIS RESULTS ****************" in line:
#             result_status = Status.SUCCESS
#         if "** OS Error:" in line:
#             result_status = Status.TIMEOUT
#         if "BRUNCH_STAT" in line:
#             parts = line.strip().split()
#             name = parts[1]
#             if name == "Clam":
#                 number = float(parts[2])
#                 if result_status == Status.CLAM_ERROR:
#                     pairs.append([dirpath.strip().split('/')[1], "error"])
#                 elif result_status == Status.TIMEOUT:
#                     pairs.append([dirpath.strip().split('/')[1], "timeout"])
#                 else:
#                     pairs.append([dirpath.strip().split('/')[1], number])
#     draw_results_table(pairs, result_dir)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='in and out files')
    parser.add_argument('logfile', type=str, help=".log file to read from")
    parser.add_argument(
        '--outfile',
        '-o',
        type=str,
        default="brunch_stat.csv",
        help="ouput csv filename")
    args = parser.parse_args()
    data = read_brunchstat_from_log(args.logfile)
    write_brunchstat_into_csv(data, args.outfile)