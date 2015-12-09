all: ../../public_html/hw3.cgi

../../public_html/hw3.cgi: hw3.c
	gcc $< -o $@

clean:
	rm ../../public_html/hw3.cgi
