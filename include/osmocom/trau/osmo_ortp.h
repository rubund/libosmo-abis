#ifndef _OSMO_ORTP_H
#define _OSMO_ORTP_H

#include <stdint.h>

#include <osmocom/core/linuxlist.h>
#include <osmocom/core/select.h>

/* we cannot include ortp/ortp.h here, as they also use 'struct msgb' */
struct _RtpSession;

/*! \brief default duration of a 20ms GSM codec frame */
#define GSM_RTP_DURATION 160

/*! \brief standard payload type for GSM Full Rate (FR) */
#define RTP_PT_GSM_FULL 3
/*! \brief Osmocom pseudo-static paylaod type for Half Rate (HR) */
#define RTP_PT_GSM_HALF 96
/*! \brief Osmocom pseudo-static paylaod type for Enhanced Full Rate (EFR) */
#define RTP_PT_GSM_EFR 97
/*! \brief Osmocom pseudo-static paylaod type for Adaptive Multi Rate (AMR) */
#define RTP_PT_AMR 98

/*! \brief Parameter to osmo_rtp_socket_param_set() */
enum osmo_rtp_param {
	OSMO_RTP_P_JITBUF = 1,
	OSMO_RTP_P_JIT_ADAP,
};

/*! \brief Flag to indicate the socket is in polling-only mode */
#define OSMO_RTP_F_POLL		0x0001

/*! \brief A structure representing one RTP socket */
struct osmo_rtp_socket {
	/*! \biref list header for global list of sockets */
	struct llist_head list;

	/*! \brief libortp RTP session pointer */
	struct _RtpSession *sess;
	/*! \brief Osmo file descriptor for RTP socket FD */
	struct osmo_fd rtp_bfd;
	/*! \brief Osmo file descriptor for RTCP socket FD */
	struct osmo_fd rtcp_bfd;

	/*! \brief callback for incoming data */
	void (*rx_cb)(struct osmo_rtp_socket *rs, const uint8_t *payload,
		      unsigned int payload_len);

	/*! \brief Receive user timestamp, to be incremented by user */
	uint32_t rx_user_ts;

	/*! \brief Transmit timestamp, incremented by library */
	uint32_t tx_timestamp;

	/*! \brief Flags like OSMO_RTP_F_POLL */
	unsigned int flags;

	void *priv;
};

void osmo_rtp_init(void *ctx);
struct osmo_rtp_socket *osmo_rtp_socket_create(void *talloc_ctx, unsigned int flags);
int osmo_rtp_socket_bind(struct osmo_rtp_socket *rs, const char *ip, int port);
int osmo_rtp_socket_connect(struct osmo_rtp_socket *rs, const char *ip, uint16_t port);
int osmo_rtp_socket_set_pt(struct osmo_rtp_socket *rs, int payload_type);
int osmo_rtp_socket_free(struct osmo_rtp_socket *rs);
int osmo_rtp_send_frame(struct osmo_rtp_socket *rs, const uint8_t *payload,
			unsigned int payload_len, unsigned int duration);
int osmo_rtp_socket_poll(struct osmo_rtp_socket *rs);

int osmo_rtp_get_bound_ip_port(struct osmo_rtp_socket *rs,
			       uint32_t *ip, int *port);
int osmo_rtp_get_bound_addr(struct osmo_rtp_socket *rs,
			    const char **addr, int *port);
int osmo_rtp_socket_set_param(struct osmo_rtp_socket *rs,
			      enum osmo_rtp_param param, int val);

void osmo_rtp_socket_log_stats(struct osmo_rtp_socket *rs,
				int subsys, int level,
				const char *pfx);

#endif /* _OSMO_ORTP_H */
