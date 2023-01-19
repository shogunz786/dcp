def get_statistics(input_list):
    sorted_list = sorted(input_list)
    list_len = len(input_list)

    mean = sum(sorted_list)/list_len
    mid = (list_len-1)//2
    median = sorted_list[mid]
    if list_len%2 == 0:
        median = (sorted_list[mid]+sorted_list[mid+1])/2

    number_counts = {x: sorted_list.count(x) for x in set(sorted_list)}
    mode = max(number_counts.keys(), key=lambda uniq_num: number_counts[uniq_num])
    sample_variance = sum([(num-mean)**2/(list_len-1) for num in sorted_list])
    sample_std_dev = sample_variance ** 0.5

    mean_std_err = sample_std_dev/list_len ** 0.5
    z_score_std_err = 1.96 * mean_std_err
    mean_confience_interval = (mean-z_score_std_err, mean+z_score_std_err)
    
    return {
        "mean": mean,
        "median": median,
        "mode": mode,
        "sample_variance": sample_variance,
        "sample_standard_deviation": sample_std_dev,
        "mean_confidence_interval": mean_confience_interval,
    }
