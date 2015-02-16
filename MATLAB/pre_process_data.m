
clear all
close all
                                                
NUM_CORES = 8;
DATA_COLUMNS = 11;
DATA_SIZE = 1000;

monitor_stats_data_cpu  = zeros(DATA_SIZE, DATA_COLUMNS, NUM_CORES);

monitored_cpus = [];

colors = ["r","b","k","y","m","c","g","w"];

for i = 1 : NUM_CORES
	filename  = strcat("monitor_stats_data_cpu_" , num2str( i -1 ) );
	if (exist(filename,'file'))
		monitored_cpus = [monitored_cpus i];
		a = importdata(filename);
		[size1(i) size2(i)] = size(a);
		monitor_stats_data_cpu(1:size1(i),1:size2(i),i) = a ;
		clear a;
	end
end
