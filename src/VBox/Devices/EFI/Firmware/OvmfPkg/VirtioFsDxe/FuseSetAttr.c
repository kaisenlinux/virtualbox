  IN OUT VIRTIO_FS  *VirtioFs,
  IN     UINT64     NodeId,
  IN     UINT64     *Size      OPTIONAL,
  IN     UINT64     *Atime     OPTIONAL,
  IN     UINT64     *Mtime     OPTIONAL,
  IN     UINT32     *Mode      OPTIONAL
  VIRTIO_FS_FUSE_REQUEST              CommonReq;
  VIRTIO_FS_FUSE_SETATTR_REQUEST      AttrReq;
  VIRTIO_FS_IO_VECTOR                 ReqIoVec[2];
  VIRTIO_FS_SCATTER_GATHER_LIST       ReqSgList;
  VIRTIO_FS_FUSE_RESPONSE             CommonResp;
  VIRTIO_FS_FUSE_GETATTR_RESPONSE     GetAttrResp;
  VIRTIO_FS_FUSE_ATTRIBUTES_RESPONSE  AttrResp;
  VIRTIO_FS_IO_VECTOR                 RespIoVec[3];
  VIRTIO_FS_SCATTER_GATHER_LIST       RespSgList;
  EFI_STATUS                          Status;
  Status = VirtioFsFuseNewRequest (
             VirtioFs,
             &CommonReq,
             ReqSgList.TotalSize,
             VirtioFsFuseOpSetAttr,
             NodeId
             );



    DEBUG ((
      DEBUG_ERROR,
      "%a: Label=\"%s\" NodeId=%Lu",
      __func__,
      VirtioFs->Label,
      NodeId
      ));




    DEBUG ((DEBUG_ERROR, " Errno=%d\n", CommonResp.Error));
