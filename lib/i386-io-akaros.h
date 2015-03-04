/* Akaros support */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
static int iob = -1;
static int iow = -1;
static int iol = -1;

static int
intel_setup_io(struct pci_access *a UNUSED)
{
	iol = open("#P/iol", O_RDWR);
	iow = open("#P/iow", O_RDWR);
	iob = open("#P/iob", O_RDWR);
	if (iob == -1 || iow == -1 || iol == -1) {
		if (iob != -1)
			close(iob);
		if (iow != -1)
			close(iow);
		if (iol != -1)
			close(iol);
		return 0;
	}
	return 1;
}

static inline int
intel_cleanup_io(struct pci_access *a UNUSED)
{
	close(iol);
	close(iow);
	close(iob);
	return 1;
}

static unsigned int inl(int off)
{
	static unsigned int buf;
	int rv;

	rv = pread(iol, &buf, 4, off);
	if (rv != 4) {
		return -1;
	}
	return buf;
}

static unsigned int inw(int off)
{
	static unsigned short buf;
	int rv;

	rv = pread(iow, &buf, 2, off);
	if (rv != 2) {
		return -1;
	}
	return buf;
}

static unsigned int inb(int off)
{
	static unsigned char buf;
	int rv;

	rv = pread(iob, &buf, 1, off);
	if (rv != 1) {
		return -1;
	}
	return buf;
}

static void outl(unsigned int val, int off)
{
	(void) pwrite(iol, &val, 4, off);
}

static void outw(unsigned short val, int off)
{
	(void)pwrite(iow, &val, 2, off);
}

static void outb(unsigned char val, int off)
{
	(void)pwrite(iob, &val, 1, off);
}
