#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include "image_element.h"

//Full-color
//Color-transition (speed > 0 == moving)
//

void init_test(struct image_element *img)
{
	int i;
	img->PixelMatrix = (struct sPixel*)malloc(sizeof(struct sPixel)*screenWidth*screenHeight);
	img->data = (void*)malloc(sizeof(short));
	short *testCounter = (short*)img->data;
	*testCounter = 0;

	struct sPixel blankPixel;

	blankPixel.red = 0;
	blankPixel.green = 0;
	blankPixel.blue = 0;

	for (i = 0; i < screenWidth*screenHeight; i++)
	{
		img->PixelMatrix[i] = blankPixel;
	}
	img->PixelMatrix[0] = img->colors[0];
	img->lastFrame = 0;
}

void update_test(struct image_element *img, int frame) 
{
	if (frame > img->lastFrame)
	{
		short *cnt = (short*)img->data;
		img->lastFrame = frame;
		//img->PixelMatrix[*cnt] = blankPixel;
		*cnt = ++(*cnt) % (screenWidth*screenHeight);
		img->PixelMatrix[*cnt] = img->colors[0];
	}
}

struct artnetData
{
	char* msg;
	int bytes;
	int sock;	
};

void init_artnet( struct image_element *img)
{
	char message[3090];
        int sock;
        struct sockaddr_in name;
        struct hostent *hp, *gethostbyname();
        int bytes;

	int i;
        img->PixelMatrix = (struct sPixel*)malloc(sizeof(struct sPixel)*screenWidth*screenHeight);
        img->data = (void*)malloc(sizeof(struct artnetData*));
        
	struct sPixel blankPixel;
	blankPixel.red = 0;
	blankPixel.green = 0;
	blankPixel.blue = 0;

        for (i = 0; i < screenWidth*screenHeight; i++)
        {
                img->PixelMatrix[i] = blankPixel;
        }
        img->PixelMatrix[0] = img->colors[0];
        img->lastFrame = 0;

	struct artnetData* data = (struct artnetData*)img->data;//(struct artnetData*)malloc(sizeof(struct artnetData));

        printf("Listen activating.\n");

        /* Create socket from which to read */
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)   {
                perror("Opening datagram socket");
                exit(1);
        }

        /* Bind our local address so that the client can send to us */
        bzero((char *) &name, sizeof(name));
        name.sin_family = AF_INET;
        name.sin_addr.s_addr = htonl(INADDR_ANY);
        name.sin_port = htons(SERVER_PORT);

        if (bind(sock, (struct sockaddr *) &name, sizeof(name))) {
                perror("Binding datagram socket");
                exit(1);
        }

        printf("Socket has port number #%d\n", ntohs(name.sin_port));
	if(read(sock, message, 1) > 0)
		perror("lol");	

	data->msg = message;
	data->sock = sock;	
        /*while ((bytes = read(sock, message, 3090)) > 0) {
                message[bytes] = '\0';
                printf("recv: %s\n", message);
        }*/

        //close(sock);
}

void artnet_to_RBG(char* data, char universe, struct image_element *img)
{
        int n;
        for(n = 0; n < 128; n++)
        {
                img->PixelMatrix[(128*(n%8))+(16*(universe-1)+n/8)].red = (short)data[n*3];
                img->PixelMatrix[(128*(n%8))+(16*(universe-1)+n/8)].green = (short)data[1+n*3];
                img->PixelMatrix[(128*(n%8))+(16*(universe-1)+n/8)].blue = (short)data[2+n*3];
		//perror("hej");
        }
}

void update_artnet(struct image_element *img, int frame)
{
        struct artnetData* data = img->data;
        char universe;
	//perror("before");
        while ((data->bytes = read(data->sock, data->msg, 512)) > 0) {
                //perror("data read");
		universe = data->msg[14];
                //data->msg = &data->msg[19];
                artnet_to_RBG(&data->msg[19], universe, img);
		break;
        }
	//perror("after");
}

