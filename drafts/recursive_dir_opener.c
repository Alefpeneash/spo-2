void recursive_dir_opener(elems, count)
char** elems;
int* count;
{
	int i = *count - 1;	
	
	while(elems[i] != 0)
	{
		//printf("%s\n", elems[i]);
		struct stat *file_stat = malloc(sizeof(struct stat)); 
		stat(elems[i], file_stat); 
		if (S_ISDIR(file_stat->st_mode))
		{	
			free(file_stat);		
			break;
		}
		free(file_stat);
		i++;
	}

	DIR *dir = opendir(elems[i]);
	struct dirent *ent = malloc(sizeof(struct dirent));

	while ((ent = readdir(dir)) != NULL)
	{
		if ((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0))
			continue;
		//printf("%s\n", ent->d_name);
		*count = *count + 1;
		elems = realloc(elems, *count * sizeof(char*));
		elems[*count - 1] = malloc((strlen(ent->d_name) + (strlen(elems[i])) + 2) * sizeof(char));
		strcpy(elems[*count - 1], elems[i]);
		strcat(elems[*count - 1], "/");
		strcat(elems[*count - 1], ent->d_name);

		struct stat *file_stat = malloc(sizeof(struct stat));
		stat(elems[*count - 1], file_stat);
		if (S_ISDIR(file_stat->st_mode))
		{
			free(file_stat);
			recursive_dir_opener(elems, count);
		}
		free(file_stat);
	}
	closedir(dir);
}

