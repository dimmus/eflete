#!/usr/bin/python
import os
import os.path
import argparse

def parse_doc(html_file):
	with open(html_file) as f:
		lines = list(f)
		for i in range(0, len(lines)):
			l = lines[i]
			if '<div class="summary">' in l:
				return
			if 'class="title"' in l:
				start = l.find('>') + 1
				FUNC_NAME = l[start:l.find('<', start)]
				start = l.find('.html">')
				TEST_MODULE_NAME = l[start + 7:l.find('<', start)] # start + len('.html">')
				break
		for i in range (i, len(lines)):
			l = lines[i]
			if '<p>' in l:
				MODULE_NAME = l[3:-6]# len('<p>'), len(' </p>\n')
				break
		j = i + 2
		for j in range (i + 2, len(lines)):
			if '</table>' in lines[j]:
				break
		file_name = "./" + TEST_MODULE_NAME + ".tmp"
		new = not os.path.exists(file_name)
		try:
			with open(file_name, 'a') as out:
				if (new):
					out.write('<tr><td>Module name</td><td>' + MODULE_NAME + '</td>\n')
					out.write('<tr><td>Function Name</td><td>Test Case ID</td>' +
							  '<td>Test Case Description</td><td>Input</td>' +
							  '<td>Expected Result</td><td>Execution Result</td>' +
							  '<td>Pass/Fail</td></tr>\n')
				for i in range (i + 2, j):
					out.write(lines[i])
		except IOError:
			print "Incorrect test doc: \"" + html_file + "\""

def walk_dirs(prefix):
	htmls = []
	for d in os.listdir(prefix):
		if (os.path.isdir(prefix + d) and d != 'search'):
			for d1 in os.listdir(prefix + d):
				for h1 in os.listdir(os.path.join(prefix, d, d1)):
					html = os.path.join(prefix, d, d1, h1)
					if html.endswith('.html'):
						parse_doc(html)

def gen_report(logs_dir, output_file):
	with open(output_file, 'w') as out:
		out.write('<html><body><table border="1">\n')
		for tmp_file in os.listdir('.'):
			if ((not os.path.isdir(tmp_file)) and (tmp_file.endswith('.tmp'))):
				try:
					with open(os.path.join(logs_dir, tmp_file[:-4].lower() + '.log')) as log:
						lines = list(log)
						for i in range(0, len(lines)):
							if ('Checks:' in lines[i]) and ('Failures:' in lines[i]) and ('Errors:' in lines[i]):
								log_start = i + 1
								break
						with open(tmp_file) as tmp:
							tmp_lines = list(tmp)
							out.write(tmp_lines[0])
							out.write(tmp_lines[1])
							i = 2
							try:
								while i < len(tmp_lines):
									test_id = tmp_lines[i + 1].split("</td><td>", 3)[1][:-1]
									while not '_REAL_RESULT_' in tmp_lines[i]:
										out.write(tmp_lines[i])
										i = i + 1
									tmp_split = tmp_lines[i].split("</td><td>", 5)
									for j in range(log_start, len(lines)):
										if test_id in lines[j]:
											log_split = lines[j].split(':', 7)
											if log_split[2] == 'P':
												result = tmp_split[2]
												passed = '<td bgcolor="#00FF00">Pass</td>'
											elif log_split[2] == 'F':
												result = log_split[6]
												passed = '<td bgcolor="#FF0000">Fail</td>'
											elif log_split[2] == 'E':
												if 'Segmentation fault' in log_split[6]:
													result = 'Segmentation fault'
												else:
													result = log_split[6]
												passed = '<td bgcolor="#FF0000">Fail</td>'
											break
									out.write(tmp_split[0] + '</td><td>')
									out.write(tmp_split[1] + '</td><td>')
									out.write(tmp_split[2] + '</td><td>')
									out.write(result + '</td>')
									out.write(passed + '</tr>')
									i = i + 3
							except IndexError:
								print "Incorrect doc structure in \"" + tmp_file + "\""
							out.write('<tr><td> </td></tr>')
				except IOError:
					print "There is no test log for \"" + tmp_file + "\""
		out.write('</table></body></html>')
		print "Saved into \"" + output_file + "\""

def main():
	parser = argparse.ArgumentParser(description='Generate UTC report')
	parser.add_argument('html', help='path to the tests documentation directory')
	parser.add_argument('log', help='path to the tests log directory')
	parser.add_argument('-c', '--clean', action='store_true', help='removes all *.tmp files from current directory before processing')
	parser.add_argument('-o', '--output', default='report.html', help='output file name. Default name: report.html')
	args = parser.parse_args()
	
	for f in os.listdir('.'):
		if not os.path.isdir(f) and f.endswith('.tmp'):
			if args.clean:
				os.remove(f)
			else:
				print 'ERROR: *.tmp files found in current directory. Remove them manualy or add --clean flag'
				return
	if not os.path.isdir(args.html):
		print 'ERROR: "{}" is not directory'.format(args.html)
		return
	if not os.path.isdir(args.log):
		print 'ERROR: "{}" is not directory'.format(args.log)
		return
	walk_dirs(args.html)
	gen_report(args.log, args.output)
	for f in os.listdir('.'):
		if not os.path.isdir(f) and f.endswith('.tmp'):
			os.remove(f)

if __name__ == '__main__':
	main()
