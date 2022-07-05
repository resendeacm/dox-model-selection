% Calibration: Maximum Likelihood
function main(level)
	% Calibration File: Results
	addpath('./Output/')
	cal_rawChain_ml
	
	% Evaluation
	log_last_chain = sprintf('cycle_cal_ip_ml_%d_rawLogLikelihood_unified', level);
	raw_last_chain = sprintf('cycle_cal_ip_ml_%d_rawChain_unified', level);
	log_eval_chain = eval(log_last_chain);
	raw_eval_chain = eval(raw_last_chain);
	
	% Maximum Likelihood
	log_mlike = max(log_eval_chain);
	pos_mlike = find(log_eval_chain == log_mlike);
	
	% Output: Maximum Likelihood
	filename = fopen('mlike.out', 'w');
	fprintf(filename, '%.4f', log_mlike);
	fclose(filename);
	
	% Output: MPV
	format long;
	filename = fopen('mpvs.out', 'a');
	fprintf(filename, '%e\t', raw_eval_chain(pos_mlike(1), :));
	save('posterior.log', 'raw_eval_chain', '-ascii');
	fclose(filename);
end
