#include <stdio.h>
#include <string.h>

#include <git2.h>

static void check_error(int error_code, const char *action)
{
	if (!error_code)
		return;

	const git_error *error = giterr_last();
	fprintf(stderr, "Error %d %s: %s\n", -error_code, action,
	        (error && error->message) ? error->message : "???");
	exit(1);
}

int main (int argc, char **argv)
{
	int error;
	git_repository *repo;
	git_revwalk *walk;
	git_oid oid;
	char buf[41];

	error = git_repository_open_ext(&repo, ".", 0, NULL);
	check_error(error, "opening repository");

	error = git_revwalk_new(&walk, repo);
	check_error(error, "allocating revwalk");
	error = git_revwalk_parseopts(walk, argc-1, argv+1);
	check_error(error, "parsing options");

	while (!git_revwalk_next(&oid, walk)) {
		git_oid_fmt(buf, &oid);
		buf[40] = '\0';
		printf("%s\n", buf);
	}

	return 0;
}

